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



#include "vbahelper/vbawindowbase.hxx"
#include "vbahelper/helperdecl.hxx"
#include <com/sun/star/awt/PosSize.hpp>

using namespace ::com::sun::star;
using namespace ::ooo::vba;

VbaWindowBase::VbaWindowBase(
        const uno::Reference< XHelperInterface >& xParent,
        const uno::Reference< uno::XComponentContext >& xContext,
        const css::uno::Reference< css::frame::XModel >& xModel,
        const uno::Reference< frame::XController >& xController ) throw (uno::RuntimeException) :
    WindowBaseImpl_BASE( xParent, xContext ),
    m_xModel( xModel, uno::UNO_SET_THROW )
{
    construct( xController );
}

VbaWindowBase::VbaWindowBase( uno::Sequence< uno::Any > const & args,
        uno::Reference< uno::XComponentContext > const & xContext ) throw (uno::RuntimeException) :
    WindowBaseImpl_BASE( getXSomethingFromArgs< XHelperInterface >( args, 0, false ), xContext ),
    m_xModel( getXSomethingFromArgs< frame::XModel >( args, 1, false ) )
{
    construct( getXSomethingFromArgs< frame::XController >( args, 2 ) );
}

sal_Bool SAL_CALL
VbaWindowBase::getVisible() throw (uno::RuntimeException)
{
    return getWindow2()->isVisible();
}

void SAL_CALL
VbaWindowBase::setVisible( sal_Bool _visible ) throw (uno::RuntimeException)
{
	getWindow2()->setVisible( _visible );
}

void setPosSize( const uno::Reference< awt::XWindow >& xWindow, sal_Int32 nValue, sal_Int16 nFlag )
{
	css::awt::Rectangle aRect = xWindow->getPosSize();
	switch( nFlag )
	{
		case css::awt::PosSize::X:
			xWindow->setPosSize( nValue, aRect.Y,	0, 0, css::awt::PosSize::X );
			break;
		case css::awt::PosSize::Y:
			xWindow->setPosSize( aRect.X, nValue,	0, 0, css::awt::PosSize::Y );
			break;
		case css::awt::PosSize::WIDTH:
			xWindow->setPosSize( 0, 0,	nValue, aRect.Height, css::awt::PosSize::WIDTH );
			break;
		case css::awt::PosSize::HEIGHT:
			xWindow->setPosSize( 0, 0,	aRect.Width, nValue, css::awt::PosSize::HEIGHT );
			break;
		default:
			break;
	}
}

sal_Int32 SAL_CALL
VbaWindowBase::getHeight() throw (uno::RuntimeException)
{
	return getWindow()->getPosSize().Height;
}

void SAL_CALL 
VbaWindowBase::setHeight( sal_Int32 _height ) throw (uno::RuntimeException)
{
	setPosSize( getWindow(), _height, css::awt::PosSize::HEIGHT );
}

sal_Int32 SAL_CALL 
VbaWindowBase::getLeft() throw (uno::RuntimeException)
{
	return getWindow()->getPosSize().X;
}

void SAL_CALL 
VbaWindowBase::setLeft( sal_Int32 _left ) throw (uno::RuntimeException)
{
	setPosSize( getWindow(), _left, css::awt::PosSize::X );
}

sal_Int32 SAL_CALL 
VbaWindowBase::getTop() throw (uno::RuntimeException)
{
	return getWindow()->getPosSize().Y;
}

void SAL_CALL 
VbaWindowBase::setTop( sal_Int32 _top ) throw (uno::RuntimeException)
{
	setPosSize( getWindow(), _top, css::awt::PosSize::Y );
}

sal_Int32 SAL_CALL 
VbaWindowBase::getWidth() throw (uno::RuntimeException)
{
	return getWindow()->getPosSize().Width;
}

void SAL_CALL 
VbaWindowBase::setWidth( sal_Int32 _width ) throw (uno::RuntimeException)
{
	setPosSize( getWindow(), _width, css::awt::PosSize::WIDTH );
}

rtl::OUString& 
VbaWindowBase::getServiceImplName()
{
	static rtl::OUString sImplName( RTL_CONSTASCII_USTRINGPARAM("VbaWindowBase") );
	return sImplName;
}

uno::Sequence< rtl::OUString > 
VbaWindowBase::getServiceNames()
{
	static uno::Sequence< rtl::OUString > aServiceNames;
	if ( aServiceNames.getLength() == 0 )
	{
		aServiceNames.realloc( 1 );
		aServiceNames[ 0 ] = rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("ooo.vba.VbaWindowBase" ) );
	}
	return aServiceNames;
}

uno::Reference< frame::XController > VbaWindowBase::getController() throw (css::uno::RuntimeException)
{
    return uno::Reference< frame::XController >( m_xController, uno::UNO_SET_THROW );
}

uno::Reference< awt::XWindow > VbaWindowBase::getWindow() throw (uno::RuntimeException)
{
    return uno::Reference< awt::XWindow >( m_xWindow, uno::UNO_SET_THROW );
}

uno::Reference< awt::XWindow2 > VbaWindowBase::getWindow2() throw (uno::RuntimeException)
{
    return uno::Reference< awt::XWindow2 >( getWindow(), uno::UNO_QUERY_THROW );
}

void VbaWindowBase::construct( const uno::Reference< frame::XController >& xController ) throw (uno::RuntimeException)
{
    if( !xController.is() ) throw uno::RuntimeException();
    uno::Reference< frame::XFrame > xFrame( xController->getFrame(), uno::UNO_SET_THROW );
    uno::Reference< awt::XWindow > xWindow( xFrame->getContainerWindow(), uno::UNO_SET_THROW );
    m_xController = xController;
    m_xWindow = xWindow;
}
