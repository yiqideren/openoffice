/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_fpicker.hxx"

#include "asyncfilepicker.hxx"
#include "iodlg.hxx"
#include "svtools/fileview.hxx"
#include <tools/debug.hxx>

#include <memory>

//........................................................................
namespace svt
{
//........................................................................

	//====================================================================
	//= AsyncPickerAction
	//====================================================================
    DBG_NAME( AsyncPickerAction )
	//--------------------------------------------------------------------
    AsyncPickerAction::AsyncPickerAction( SvtFileDialog* _pDialog, SvtFileView* _pView, const Action _eAction )
        :m_refCount ( 0        )
        ,m_eAction  ( _eAction )
        ,m_pView    ( _pView   )
        ,m_pDialog  ( _pDialog )
        ,m_bRunning ( false    )
    {
        DBG_CTOR( AsyncPickerAction, NULL );
        DBG_ASSERT( m_pDialog, "AsyncPickerAction::AsyncPickerAction: invalid dialog!" );
        DBG_ASSERT( m_pView, "AsyncPickerAction::AsyncPickerAction: invalid view!" );
    }

    //--------------------------------------------------------------------
    AsyncPickerAction::~AsyncPickerAction()
    {
        DBG_DTOR( AsyncPickerAction, NULL );
    }

    //--------------------------------------------------------------------
    oslInterlockedCount SAL_CALL AsyncPickerAction::acquire()
    {
        return osl_incrementInterlockedCount( &m_refCount );
    }
    
    //--------------------------------------------------------------------
    oslInterlockedCount SAL_CALL AsyncPickerAction::release()
    {
        if ( 0 == osl_decrementInterlockedCount( &m_refCount ) )
        {
            delete this;
            return 0;
        }
        return m_refCount;
    }

	//--------------------------------------------------------------------
    void AsyncPickerAction::cancel()
    {
        DBG_TESTSOLARMUTEX();
            // if this asserts, we'd need to have an own mutex per instance

        OSL_ENSURE( m_bRunning, "AsyncPickerAction::cancel: not running" );
        if ( m_pView )
            m_pView->CancelRunningAsyncAction();
    }

	//--------------------------------------------------------------------
    void AsyncPickerAction::execute( 
		const String& _rURL, 
		const String& _rFilter, 
		sal_Int32 _nMinTimeout, 
		sal_Int32 _nMaxTimeout, 
		const OUStringList& rBlackList )
    {
        DBG_TESTSOLARMUTEX();
            // if this asserts, we'd need to have an own mutex per instance

        sal_Int32 nMinTimeout = _nMinTimeout;
        sal_Int32 nMaxTimeout = _nMaxTimeout;
        // normalizations
        if ( nMinTimeout < 0 )
            // if negative, this is considered as "do it synchronously"
            nMinTimeout = 0;
        else if ( nMinTimeout < 1000 )
            nMinTimeout = 1000;
        if ( nMaxTimeout <= nMinTimeout )
            nMaxTimeout = nMinTimeout + 30000;

        ::std::auto_ptr< FileViewAsyncAction > pActionDescriptor;
        if ( nMinTimeout )
        {
            pActionDescriptor.reset( new FileViewAsyncAction );
            pActionDescriptor->nMinTimeout = nMinTimeout;
            pActionDescriptor->nMaxTimeout = nMaxTimeout;
            pActionDescriptor->aFinishHandler = LINK( this, AsyncPickerAction, OnActionDone );
        }

        FileViewResult eResult = eFailure;
        m_sURL = _rURL;
        switch ( m_eAction )
        {
        case ePrevLevel:
            eResult = m_pView->PreviousLevel( pActionDescriptor.get() );
            break;

        case eOpenURL:
            eResult = m_pView->Initialize( _rURL, _rFilter, pActionDescriptor.get(), rBlackList );
            break;

        case eExecuteFilter:
            // preserve the filename (FS: why?)
            m_sFileName = m_pDialog->getCurrentFileText();
            // execute the new filter
            eResult = m_pView->ExecuteFilter( _rFilter, pActionDescriptor.get() );
            break;

        default:
            DBG_ERROR( "AsyncPickerAction::execute: unknown action!" );
            break;
        }

        acquire();
        if ( ( eResult == eSuccess ) || ( eResult == eFailure ) )
        {
            // the handler is only called if the action could not be finished within
            // the given minimum time period. In case of success, we need to call it
            // explicitly
            OnActionDone( reinterpret_cast< void* >( eResult ) );
        }
        else if ( eResult == eStillRunning )
        {
            m_bRunning = true;
            m_pDialog->onAsyncOperationStarted();
        }
    }

	//--------------------------------------------------------------------
	IMPL_LINK( AsyncPickerAction, OnActionDone, void*, pEmptyArg )
    {
        DBG_TESTSOLARMUTEX();
            // if this asserts, we'd need to have an own mutex per instance

        FileViewResult eResult = static_cast< FileViewResult >( reinterpret_cast< sal_IntPtr >( pEmptyArg ) );
        OSL_ENSURE( eStillRunning != eResult, "AsyncPickerAction::OnActionDone: invalid result!" );

        // release once (since we acquired in |execute|), but keep alive until the
        // end of the method
        ::rtl::Reference< AsyncPickerAction > xKeepAlive( this );
        release();

        m_pDialog->onAsyncOperationFinished();
        m_bRunning = true;

        if ( eFailure == eResult )
            // TODO: do we need some kind of cleanup here?
            return 0L;

        if ( eTimeout == eResult )
        {
            m_pDialog->displayIOException( m_sURL, ::com::sun::star::ucb::IOErrorCode_CANT_READ );
            return 0L;
        }

        OSL_ENSURE( eSuccess == eResult, "AsyncPickerAction::OnActionDone: what else valid results are there?" );

        switch ( m_eAction )
        {
        case ePrevLevel:
        case eOpenURL:
            m_pDialog->UpdateControls( m_pView->GetViewURL() );
            break;

        case eExecuteFilter:
	        // restore the filename
	        m_pView->SetNoSelection();
            m_pDialog->setCurrentFileText( m_sFileName, true );

	        // notify listeners
	        m_pDialog->FilterSelect();
            break;

        default:
            DBG_ERROR( "AsyncPickerAction::OnActionDone: unknown action!" );
            break;
        }

        return 1L;
    }

//........................................................................
} // namespace svt
//........................................................................

