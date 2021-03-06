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


#ifndef INCLUDED_unotools_FONTOPTIONS_HXX
#define INCLUDED_unotools_FONTOPTIONS_HXX

//_________________________________________________________________________________________________________________
//	includes
//_________________________________________________________________________________________________________________

#include "unotools/unotoolsdllapi.h"
#include <sal/types.h>
#include <osl/mutex.hxx>
#include <rtl/ustring.hxx>
#include <unotools/options.hxx>

//_________________________________________________________________________________________________________________
//	forward declarations
//_________________________________________________________________________________________________________________

/*-************************************************************************************************************//**
	@short			forward declaration to our private date container implementation
	@descr			We use these class as internal member to support small memory requirements.
					You can create the container if it is necessary. The class which use these mechanism
					is faster and smaller then a complete implementation!
*//*-*************************************************************************************************************/

class SvtFontOptions_Impl;

//_________________________________________________________________________________________________________________
//	declarations
//_________________________________________________________________________________________________________________

/*-************************************************************************************************************//**
	@short			collect informations about font features
	@descr          -

	@implements		-
	@base			-

	@devstatus		ready to use
*//*-*************************************************************************************************************/

class UNOTOOLS_DLLPUBLIC SvtFontOptions: public utl::detail::Options
{
	//-------------------------------------------------------------------------------------------------------------
	//	public methods
	//-------------------------------------------------------------------------------------------------------------

	public:

		//---------------------------------------------------------------------------------------------------------
		//	constructor / destructor
		//---------------------------------------------------------------------------------------------------------

		/*-****************************************************************************************************//**
			@short		standard constructor and destructor
			@descr		This will initialize an instance with default values.
						We implement these class with a refcount mechanism! Every instance of this class increase it
						at create and decrease it at delete time - but all instances use the same data container!
						He is implemented as a static member ...

			@seealso	member m_nRefCount
			@seealso	member m_pDataContainer

			@param		-
			@return		-

			@onerror	-
		*//*-*****************************************************************************************************/

         SvtFontOptions();
        virtual ~SvtFontOptions();

		//---------------------------------------------------------------------------------------------------------
		//	interface
		//---------------------------------------------------------------------------------------------------------

		/*-****************************************************************************************************//**
			@short		interface methods to get and set value of config key "org.openoffice.Office.Common/Font"
			@descr		These values defines different states of font handling.

						..ReplacementTable..()	=>  Determines if the list of font replacements is applied or not.

						..FontHistory..()		=>  The last 5 fonts will be shown and the last one will be the
													first name on the list. These will be displayed in the Font name
													combo box on the Object bar.

						..FontWYSIWYG..()		=>  With this option the names of the selectable fonts
													(for example, the fonts in the Font field in the object bar)
													will be formatted as the current font.

			@seealso	-

			@param		-
			@return		-

			@onerror	No error should occurre!
		*//*-*****************************************************************************************************/

		sal_Bool	IsReplacementTableEnabled	(					) const	;
		void		EnableReplacementTable		( sal_Bool bState	)		;

		sal_Bool	IsFontHistoryEnabled		(					) const	;
		void		EnableFontHistory			( sal_Bool bState	)		;

		sal_Bool	IsFontWYSIWYGEnabled		(					) const	;
		void		EnableFontWYSIWYG			( sal_Bool bState	)		;

	//-------------------------------------------------------------------------------------------------------------
	//	private methods
	//-------------------------------------------------------------------------------------------------------------

	private:

		/*-****************************************************************************************************//**
			@short		return a reference to a static mutex
			@descr		These class use his own static mutex to be threadsafe.
						We create a static mutex only for one ime and use at different times.

			@seealso	-

			@param		-
			@return		A reference to a static mutex member.

			@onerror	-
		*//*-*****************************************************************************************************/

        UNOTOOLS_DLLPRIVATE static ::osl::Mutex& impl_GetOwnStaticMutex();

	//-------------------------------------------------------------------------------------------------------------
	//	private member
	//-------------------------------------------------------------------------------------------------------------

	private:

		/*Attention

			Don't initialize these static member in these header!
			a) Double dfined symbols will be detected ...
			b) and unresolved externals exist at linking time.
			Do it in your source only.
		 */

    	static SvtFontOptions_Impl*			m_pDataContainer	;	/// impl. data container as dynamic pointer for smaller memory requirements!
		static sal_Int32					m_nRefCount			;	/// internal ref count mechanism

};		// class SvtFontOptions

#endif  // #ifndef INCLUDED_unotools_FONTOPTIONS_HXX
