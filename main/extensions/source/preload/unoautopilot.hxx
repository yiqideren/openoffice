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



#ifndef _EXTENSIONS_PRELOAD_UNOAUTOPILOT_HXX_
#define _EXTENSIONS_PRELOAD_UNOAUTOPILOT_HXX_

#include <svtools/genericunodialog.hxx>
#include <comphelper/proparrhlp.hxx>
#include "componentmodule.hxx"
#include <cppuhelper/typeprovider.hxx>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>

//.........................................................................
namespace preload
{
//.........................................................................

	//=====================================================================
	//= IServiceInfo
	//=====================================================================
	/** interface for the SERVICEINFO template parameter of the OUnoAutoPilot class
	*/
	struct IServiceInfo
	{
	public:
		::rtl::OUString		getImplementationName() const;
		::com::sun::star::uno::Sequence< ::rtl::OUString >
							getServiceNames() const;
	};

	//=====================================================================
	//= OUnoAutoPilot
	//=====================================================================
	typedef ::svt::OGenericUnoDialog	OUnoAutoPilot_Base;
	template <class TYPE, class SERVICEINFO>
	class OUnoAutoPilot
			:public OUnoAutoPilot_Base
			,public ::comphelper::OPropertyArrayUsageHelper< OUnoAutoPilot< TYPE, SERVICEINFO > >
			,public OModuleResourceClient
	{
		OUnoAutoPilot(const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& _rxORB);

	protected:
		::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >
					m_xObjectModel;

	public:
		// XTypeProvider
		virtual ::com::sun::star::uno::Sequence<sal_Int8> SAL_CALL getImplementationId(  ) throw(::com::sun::star::uno::RuntimeException);

		// XServiceInfo
		virtual ::rtl::OUString SAL_CALL getImplementationName() throw(::com::sun::star::uno::RuntimeException);
		virtual ::comphelper::StringSequence SAL_CALL getSupportedServiceNames() throw(::com::sun::star::uno::RuntimeException);

		// XServiceInfo - static methods
		static ::com::sun::star::uno::Sequence< ::rtl::OUString > getSupportedServiceNames_Static(void) throw( ::com::sun::star::uno::RuntimeException );
		static ::rtl::OUString getImplementationName_Static(void) throw( ::com::sun::star::uno::RuntimeException );
		static ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >
				SAL_CALL Create(const ::com::sun::star::uno::Reference< com::sun::star::lang::XMultiServiceFactory >&);

		// XPropertySet
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo>  SAL_CALL getPropertySetInfo() throw(::com::sun::star::uno::RuntimeException);
		virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();

		// OPropertyArrayUsageHelper
		virtual ::cppu::IPropertyArrayHelper* createArrayHelper( ) const;

	protected:
	// OGenericUnoDialog overridables
		virtual Dialog*	createDialog(Window* _pParent);
		virtual void implInitialize(const com::sun::star::uno::Any& _rValue);
	};

#include "unoautopilot.inl"

//.........................................................................
}	// namespace dbp
//.........................................................................

#endif // _EXTENSIONS_PRELOAD_UNOAUTOPILOT_HXX_


