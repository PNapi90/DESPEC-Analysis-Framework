// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__SCN

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "./AIDA_Headers.h"
#include "./TSCNParameter.h"
#include "./FATIMA_Gain_Match.h"
#include "./PLASTIC_Data_Stream.h"
#include "./WR_Structure.h"
#include "./Events.h"
#include "./Gap_Handler.h"
#include "./PLASTIC_Calibrator.h"
#include "./FRS_Detector_System.h"
#include "./FINGER_Data_Stream.h"
#include "./FATIMA_Time_Calibration.h"
#include "./Raw_Event.h"
#include "./FINGER_Detector_System.h"
#include "./TSCNCalProc.h"
#include "./TXRSParameter.h"
#include "./FINGER_Calibrator.h"
#include "./Match.h"
#include "./TAMEX.h"
#include "./TDC_1290.h"
#include "./Time_EventBuilder.h"
#include "./GALILEO_Time_Calibration.h"
#include "./Beta_Event.h"
#include "./TSCNAnlEvent.h"
#include "./First_Test.h"
#include "./AIDA_Data_Types.h"
#include "./FATIMA_Detector_System.h"
#include "./FATIMA_Energy_Calibration.h"
#include "./Raw_Event_Storage.h"
#include "./TSCNUnpackEvent.h"
#include "./Event_Store.h"
#include "./FEBEX.h"
#include "./PLASTIC_Detector_System.h"
#include "./QDC_751.h"
#include "./TSCNAnlProc.h"
#include "./FATIMA_Event.h"
#include "./GALILEO_Energy_Calibration.h"
#include "./AIDA_Detector_System.h"
#include "./PLASTIC_Event.h"
#include "./White_Rabbit.h"
#include "./GALILEO_Detector_System_TEST.h"
#include "./TSCNCalEvent.h"
#include "./TSCNAnalysis.h"
#include "./GALILEO_Event.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TSCNParameter(void *p = 0);
   static void *newArray_TSCNParameter(Long_t size, void *p);
   static void delete_TSCNParameter(void *p);
   static void deleteArray_TSCNParameter(void *p);
   static void destruct_TSCNParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCNParameter*)
   {
      ::TSCNParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCNParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCNParameter", ::TSCNParameter::Class_Version(), "TSCNParameter.h", 22,
                  typeid(::TSCNParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCNParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TSCNParameter) );
      instance.SetNew(&new_TSCNParameter);
      instance.SetNewArray(&newArray_TSCNParameter);
      instance.SetDelete(&delete_TSCNParameter);
      instance.SetDeleteArray(&deleteArray_TSCNParameter);
      instance.SetDestructor(&destruct_TSCNParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCNParameter*)
   {
      return GenerateInitInstanceLocal((::TSCNParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCNParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TXRSParameter(void *p = 0);
   static void *newArray_TXRSParameter(Long_t size, void *p);
   static void delete_TXRSParameter(void *p);
   static void deleteArray_TXRSParameter(void *p);
   static void destruct_TXRSParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TXRSParameter*)
   {
      ::TXRSParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TXRSParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TXRSParameter", ::TXRSParameter::Class_Version(), "TXRSParameter.h", 8,
                  typeid(::TXRSParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TXRSParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TXRSParameter) );
      instance.SetNew(&new_TXRSParameter);
      instance.SetNewArray(&newArray_TXRSParameter);
      instance.SetDelete(&delete_TXRSParameter);
      instance.SetDeleteArray(&deleteArray_TXRSParameter);
      instance.SetDestructor(&destruct_TXRSParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TXRSParameter*)
   {
      return GenerateInitInstanceLocal((::TXRSParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TXRSParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Map1(void *p = 0);
   static void *newArray_Map1(Long_t size, void *p);
   static void delete_Map1(void *p);
   static void deleteArray_Map1(void *p);
   static void destruct_Map1(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Map1*)
   {
      ::Map1 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Map1 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Map1", ::Map1::Class_Version(), "TXRSParameter.h", 88,
                  typeid(::Map1), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Map1::Dictionary, isa_proxy, 4,
                  sizeof(::Map1) );
      instance.SetNew(&new_Map1);
      instance.SetNewArray(&newArray_Map1);
      instance.SetDelete(&delete_Map1);
      instance.SetDeleteArray(&deleteArray_Map1);
      instance.SetDestructor(&destruct_Map1);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Map1*)
   {
      return GenerateInitInstanceLocal((::Map1*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Map1*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TModParameter(void *p = 0);
   static void *newArray_TModParameter(Long_t size, void *p);
   static void delete_TModParameter(void *p);
   static void deleteArray_TModParameter(void *p);
   static void destruct_TModParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TModParameter*)
   {
      ::TModParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TModParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TModParameter", ::TModParameter::Class_Version(), "TXRSParameter.h", 97,
                  typeid(::TModParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TModParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TModParameter) );
      instance.SetNew(&new_TModParameter);
      instance.SetNewArray(&newArray_TModParameter);
      instance.SetDelete(&delete_TModParameter);
      instance.SetDeleteArray(&deleteArray_TModParameter);
      instance.SetDestructor(&destruct_TModParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TModParameter*)
   {
      return GenerateInitInstanceLocal((::TModParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TModParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TMWParameter(void *p = 0);
   static void *newArray_TMWParameter(Long_t size, void *p);
   static void delete_TMWParameter(void *p);
   static void deleteArray_TMWParameter(void *p);
   static void destruct_TMWParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TMWParameter*)
   {
      ::TMWParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TMWParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TMWParameter", ::TMWParameter::Class_Version(), "TXRSParameter.h", 131,
                  typeid(::TMWParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TMWParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TMWParameter) );
      instance.SetNew(&new_TMWParameter);
      instance.SetNewArray(&newArray_TMWParameter);
      instance.SetDelete(&delete_TMWParameter);
      instance.SetDeleteArray(&deleteArray_TMWParameter);
      instance.SetDestructor(&destruct_TMWParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TMWParameter*)
   {
      return GenerateInitInstanceLocal((::TMWParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TMWParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TTPCParameter(void *p = 0);
   static void *newArray_TTPCParameter(Long_t size, void *p);
   static void delete_TTPCParameter(void *p);
   static void deleteArray_TTPCParameter(void *p);
   static void destruct_TTPCParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TTPCParameter*)
   {
      ::TTPCParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TTPCParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TTPCParameter", ::TTPCParameter::Class_Version(), "TXRSParameter.h", 150,
                  typeid(::TTPCParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TTPCParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TTPCParameter) );
      instance.SetNew(&new_TTPCParameter);
      instance.SetNewArray(&newArray_TTPCParameter);
      instance.SetDelete(&delete_TTPCParameter);
      instance.SetDeleteArray(&deleteArray_TTPCParameter);
      instance.SetDestructor(&destruct_TTPCParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TTPCParameter*)
   {
      return GenerateInitInstanceLocal((::TTPCParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TTPCParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TMUSICParameter(void *p = 0);
   static void *newArray_TMUSICParameter(Long_t size, void *p);
   static void delete_TMUSICParameter(void *p);
   static void deleteArray_TMUSICParameter(void *p);
   static void destruct_TMUSICParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TMUSICParameter*)
   {
      ::TMUSICParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TMUSICParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TMUSICParameter", ::TMUSICParameter::Class_Version(), "TXRSParameter.h", 174,
                  typeid(::TMUSICParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TMUSICParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TMUSICParameter) );
      instance.SetNew(&new_TMUSICParameter);
      instance.SetNewArray(&newArray_TMUSICParameter);
      instance.SetDelete(&delete_TMUSICParameter);
      instance.SetDeleteArray(&deleteArray_TMUSICParameter);
      instance.SetDestructor(&destruct_TMUSICParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TMUSICParameter*)
   {
      return GenerateInitInstanceLocal((::TMUSICParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TMUSICParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCIParameter(void *p = 0);
   static void *newArray_TSCIParameter(Long_t size, void *p);
   static void delete_TSCIParameter(void *p);
   static void deleteArray_TSCIParameter(void *p);
   static void destruct_TSCIParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCIParameter*)
   {
      ::TSCIParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCIParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCIParameter", ::TSCIParameter::Class_Version(), "TXRSParameter.h", 198,
                  typeid(::TSCIParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCIParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TSCIParameter) );
      instance.SetNew(&new_TSCIParameter);
      instance.SetNewArray(&newArray_TSCIParameter);
      instance.SetDelete(&delete_TSCIParameter);
      instance.SetDeleteArray(&deleteArray_TSCIParameter);
      instance.SetDestructor(&destruct_TSCIParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCIParameter*)
   {
      return GenerateInitInstanceLocal((::TSCIParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCIParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TIDParameter(void *p = 0);
   static void *newArray_TIDParameter(Long_t size, void *p);
   static void delete_TIDParameter(void *p);
   static void deleteArray_TIDParameter(void *p);
   static void destruct_TIDParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TIDParameter*)
   {
      ::TIDParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TIDParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TIDParameter", ::TIDParameter::Class_Version(), "TXRSParameter.h", 231,
                  typeid(::TIDParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TIDParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TIDParameter) );
      instance.SetNew(&new_TIDParameter);
      instance.SetNewArray(&newArray_TIDParameter);
      instance.SetDelete(&delete_TIDParameter);
      instance.SetDeleteArray(&deleteArray_TIDParameter);
      instance.SetDestructor(&destruct_TIDParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TIDParameter*)
   {
      return GenerateInitInstanceLocal((::TIDParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TIDParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSIParameter(void *p = 0);
   static void *newArray_TSIParameter(Long_t size, void *p);
   static void delete_TSIParameter(void *p);
   static void deleteArray_TSIParameter(void *p);
   static void destruct_TSIParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSIParameter*)
   {
      ::TSIParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSIParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSIParameter", ::TSIParameter::Class_Version(), "TXRSParameter.h", 277,
                  typeid(::TSIParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSIParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TSIParameter) );
      instance.SetNew(&new_TSIParameter);
      instance.SetNewArray(&newArray_TSIParameter);
      instance.SetDelete(&delete_TSIParameter);
      instance.SetDeleteArray(&deleteArray_TSIParameter);
      instance.SetDestructor(&destruct_TSIParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSIParameter*)
   {
      return GenerateInitInstanceLocal((::TSIParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSIParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TMRTOFMSParameter(void *p = 0);
   static void *newArray_TMRTOFMSParameter(Long_t size, void *p);
   static void delete_TMRTOFMSParameter(void *p);
   static void deleteArray_TMRTOFMSParameter(void *p);
   static void destruct_TMRTOFMSParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TMRTOFMSParameter*)
   {
      ::TMRTOFMSParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TMRTOFMSParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TMRTOFMSParameter", ::TMRTOFMSParameter::Class_Version(), "TXRSParameter.h", 304,
                  typeid(::TMRTOFMSParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TMRTOFMSParameter::Dictionary, isa_proxy, 4,
                  sizeof(::TMRTOFMSParameter) );
      instance.SetNew(&new_TMRTOFMSParameter);
      instance.SetNewArray(&newArray_TMRTOFMSParameter);
      instance.SetDelete(&delete_TMRTOFMSParameter);
      instance.SetDeleteArray(&deleteArray_TMRTOFMSParameter);
      instance.SetDestructor(&destruct_TMRTOFMSParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TMRTOFMSParameter*)
   {
      return GenerateInitInstanceLocal((::TMRTOFMSParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TMRTOFMSParameter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCNUnpackEvent(void *p = 0);
   static void *newArray_TSCNUnpackEvent(Long_t size, void *p);
   static void delete_TSCNUnpackEvent(void *p);
   static void deleteArray_TSCNUnpackEvent(void *p);
   static void destruct_TSCNUnpackEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCNUnpackEvent*)
   {
      ::TSCNUnpackEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCNUnpackEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCNUnpackEvent", ::TSCNUnpackEvent::Class_Version(), "TSCNUnpackEvent.h", 26,
                  typeid(::TSCNUnpackEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCNUnpackEvent::Dictionary, isa_proxy, 4,
                  sizeof(::TSCNUnpackEvent) );
      instance.SetNew(&new_TSCNUnpackEvent);
      instance.SetNewArray(&newArray_TSCNUnpackEvent);
      instance.SetDelete(&delete_TSCNUnpackEvent);
      instance.SetDeleteArray(&deleteArray_TSCNUnpackEvent);
      instance.SetDestructor(&destruct_TSCNUnpackEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCNUnpackEvent*)
   {
      return GenerateInitInstanceLocal((::TSCNUnpackEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCNUnpackEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCNCalProc(void *p = 0);
   static void *newArray_TSCNCalProc(Long_t size, void *p);
   static void delete_TSCNCalProc(void *p);
   static void deleteArray_TSCNCalProc(void *p);
   static void destruct_TSCNCalProc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCNCalProc*)
   {
      ::TSCNCalProc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCNCalProc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCNCalProc", ::TSCNCalProc::Class_Version(), "TSCNCalProc.h", 26,
                  typeid(::TSCNCalProc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCNCalProc::Dictionary, isa_proxy, 4,
                  sizeof(::TSCNCalProc) );
      instance.SetNew(&new_TSCNCalProc);
      instance.SetNewArray(&newArray_TSCNCalProc);
      instance.SetDelete(&delete_TSCNCalProc);
      instance.SetDeleteArray(&deleteArray_TSCNCalProc);
      instance.SetDestructor(&destruct_TSCNCalProc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCNCalProc*)
   {
      return GenerateInitInstanceLocal((::TSCNCalProc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCNCalProc*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCNCalEvent(void *p = 0);
   static void *newArray_TSCNCalEvent(Long_t size, void *p);
   static void delete_TSCNCalEvent(void *p);
   static void deleteArray_TSCNCalEvent(void *p);
   static void destruct_TSCNCalEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCNCalEvent*)
   {
      ::TSCNCalEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCNCalEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCNCalEvent", ::TSCNCalEvent::Class_Version(), "TSCNCalEvent.h", 21,
                  typeid(::TSCNCalEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCNCalEvent::Dictionary, isa_proxy, 4,
                  sizeof(::TSCNCalEvent) );
      instance.SetNew(&new_TSCNCalEvent);
      instance.SetNewArray(&newArray_TSCNCalEvent);
      instance.SetDelete(&delete_TSCNCalEvent);
      instance.SetDeleteArray(&deleteArray_TSCNCalEvent);
      instance.SetDestructor(&destruct_TSCNCalEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCNCalEvent*)
   {
      return GenerateInitInstanceLocal((::TSCNCalEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCNCalEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCNAnlEvent(void *p = 0);
   static void *newArray_TSCNAnlEvent(Long_t size, void *p);
   static void delete_TSCNAnlEvent(void *p);
   static void deleteArray_TSCNAnlEvent(void *p);
   static void destruct_TSCNAnlEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCNAnlEvent*)
   {
      ::TSCNAnlEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCNAnlEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCNAnlEvent", ::TSCNAnlEvent::Class_Version(), "TSCNAnlEvent.h", 21,
                  typeid(::TSCNAnlEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCNAnlEvent::Dictionary, isa_proxy, 4,
                  sizeof(::TSCNAnlEvent) );
      instance.SetNew(&new_TSCNAnlEvent);
      instance.SetNewArray(&newArray_TSCNAnlEvent);
      instance.SetDelete(&delete_TSCNAnlEvent);
      instance.SetDeleteArray(&deleteArray_TSCNAnlEvent);
      instance.SetDestructor(&destruct_TSCNAnlEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCNAnlEvent*)
   {
      return GenerateInitInstanceLocal((::TSCNAnlEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCNAnlEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCNUnpackProc(void *p = 0);
   static void *newArray_TSCNUnpackProc(Long_t size, void *p);
   static void delete_TSCNUnpackProc(void *p);
   static void deleteArray_TSCNUnpackProc(void *p);
   static void destruct_TSCNUnpackProc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCNUnpackProc*)
   {
      ::TSCNUnpackProc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCNUnpackProc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCNUnpackProc", ::TSCNUnpackProc::Class_Version(), "First_Test.h", 64,
                  typeid(::TSCNUnpackProc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCNUnpackProc::Dictionary, isa_proxy, 4,
                  sizeof(::TSCNUnpackProc) );
      instance.SetNew(&new_TSCNUnpackProc);
      instance.SetNewArray(&newArray_TSCNUnpackProc);
      instance.SetDelete(&delete_TSCNUnpackProc);
      instance.SetDeleteArray(&deleteArray_TSCNUnpackProc);
      instance.SetDestructor(&destruct_TSCNUnpackProc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCNUnpackProc*)
   {
      return GenerateInitInstanceLocal((::TSCNUnpackProc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCNUnpackProc*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCNAnlProc(void *p = 0);
   static void *newArray_TSCNAnlProc(Long_t size, void *p);
   static void delete_TSCNAnlProc(void *p);
   static void deleteArray_TSCNAnlProc(void *p);
   static void destruct_TSCNAnlProc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCNAnlProc*)
   {
      ::TSCNAnlProc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCNAnlProc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCNAnlProc", ::TSCNAnlProc::Class_Version(), "TSCNAnlProc.h", 29,
                  typeid(::TSCNAnlProc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCNAnlProc::Dictionary, isa_proxy, 4,
                  sizeof(::TSCNAnlProc) );
      instance.SetNew(&new_TSCNAnlProc);
      instance.SetNewArray(&newArray_TSCNAnlProc);
      instance.SetDelete(&delete_TSCNAnlProc);
      instance.SetDeleteArray(&deleteArray_TSCNAnlProc);
      instance.SetDestructor(&destruct_TSCNAnlProc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCNAnlProc*)
   {
      return GenerateInitInstanceLocal((::TSCNAnlProc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCNAnlProc*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSCNAnalysis(void *p = 0);
   static void *newArray_TSCNAnalysis(Long_t size, void *p);
   static void delete_TSCNAnalysis(void *p);
   static void deleteArray_TSCNAnalysis(void *p);
   static void destruct_TSCNAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSCNAnalysis*)
   {
      ::TSCNAnalysis *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSCNAnalysis >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSCNAnalysis", ::TSCNAnalysis::Class_Version(), "TSCNAnalysis.h", 27,
                  typeid(::TSCNAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSCNAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::TSCNAnalysis) );
      instance.SetNew(&new_TSCNAnalysis);
      instance.SetNewArray(&newArray_TSCNAnalysis);
      instance.SetDelete(&delete_TSCNAnalysis);
      instance.SetDeleteArray(&deleteArray_TSCNAnalysis);
      instance.SetDestructor(&destruct_TSCNAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSCNAnalysis*)
   {
      return GenerateInitInstanceLocal((::TSCNAnalysis*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSCNAnalysis*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TSCNParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCNParameter::Class_Name()
{
   return "TSCNParameter";
}

//______________________________________________________________________________
const char *TSCNParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCNParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCNParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCNParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TXRSParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TXRSParameter::Class_Name()
{
   return "TXRSParameter";
}

//______________________________________________________________________________
const char *TXRSParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TXRSParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TXRSParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TXRSParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TXRSParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TXRSParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TXRSParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TXRSParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Map1::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Map1::Class_Name()
{
   return "Map1";
}

//______________________________________________________________________________
const char *Map1::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Map1*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Map1::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Map1*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Map1::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Map1*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Map1::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Map1*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TModParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TModParameter::Class_Name()
{
   return "TModParameter";
}

//______________________________________________________________________________
const char *TModParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TModParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TModParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TModParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TModParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TModParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TModParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TModParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TMWParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TMWParameter::Class_Name()
{
   return "TMWParameter";
}

//______________________________________________________________________________
const char *TMWParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMWParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TMWParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMWParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TMWParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMWParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TMWParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMWParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TTPCParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TTPCParameter::Class_Name()
{
   return "TTPCParameter";
}

//______________________________________________________________________________
const char *TTPCParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TTPCParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TTPCParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TTPCParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TTPCParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TTPCParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TTPCParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TTPCParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TMUSICParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TMUSICParameter::Class_Name()
{
   return "TMUSICParameter";
}

//______________________________________________________________________________
const char *TMUSICParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMUSICParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TMUSICParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMUSICParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TMUSICParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMUSICParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TMUSICParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMUSICParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCIParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCIParameter::Class_Name()
{
   return "TSCIParameter";
}

//______________________________________________________________________________
const char *TSCIParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCIParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCIParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCIParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCIParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCIParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCIParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCIParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TIDParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TIDParameter::Class_Name()
{
   return "TIDParameter";
}

//______________________________________________________________________________
const char *TIDParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TIDParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TIDParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TIDParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TIDParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TIDParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TIDParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TIDParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSIParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSIParameter::Class_Name()
{
   return "TSIParameter";
}

//______________________________________________________________________________
const char *TSIParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSIParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSIParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSIParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSIParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSIParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSIParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSIParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TMRTOFMSParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TMRTOFMSParameter::Class_Name()
{
   return "TMRTOFMSParameter";
}

//______________________________________________________________________________
const char *TMRTOFMSParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMRTOFMSParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TMRTOFMSParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMRTOFMSParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TMRTOFMSParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMRTOFMSParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TMRTOFMSParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMRTOFMSParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCNUnpackEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCNUnpackEvent::Class_Name()
{
   return "TSCNUnpackEvent";
}

//______________________________________________________________________________
const char *TSCNUnpackEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNUnpackEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCNUnpackEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNUnpackEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCNUnpackEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNUnpackEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCNUnpackEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNUnpackEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCNCalProc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCNCalProc::Class_Name()
{
   return "TSCNCalProc";
}

//______________________________________________________________________________
const char *TSCNCalProc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNCalProc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCNCalProc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNCalProc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCNCalProc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNCalProc*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCNCalProc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNCalProc*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCNCalEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCNCalEvent::Class_Name()
{
   return "TSCNCalEvent";
}

//______________________________________________________________________________
const char *TSCNCalEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNCalEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCNCalEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNCalEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCNCalEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNCalEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCNCalEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNCalEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCNAnlEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCNAnlEvent::Class_Name()
{
   return "TSCNAnlEvent";
}

//______________________________________________________________________________
const char *TSCNAnlEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnlEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCNAnlEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnlEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCNAnlEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnlEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCNAnlEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnlEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCNUnpackProc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCNUnpackProc::Class_Name()
{
   return "TSCNUnpackProc";
}

//______________________________________________________________________________
const char *TSCNUnpackProc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNUnpackProc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCNUnpackProc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNUnpackProc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCNUnpackProc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNUnpackProc*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCNUnpackProc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNUnpackProc*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCNAnlProc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCNAnlProc::Class_Name()
{
   return "TSCNAnlProc";
}

//______________________________________________________________________________
const char *TSCNAnlProc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnlProc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCNAnlProc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnlProc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCNAnlProc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnlProc*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCNAnlProc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnlProc*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSCNAnalysis::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSCNAnalysis::Class_Name()
{
   return "TSCNAnalysis";
}

//______________________________________________________________________________
const char *TSCNAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnalysis*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSCNAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnalysis*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSCNAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnalysis*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSCNAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSCNAnalysis*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TSCNParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCNParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCNParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCNParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCNParameter(void *p) {
      return  p ? new(p) ::TSCNParameter : new ::TSCNParameter;
   }
   static void *newArray_TSCNParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TSCNParameter[nElements] : new ::TSCNParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCNParameter(void *p) {
      delete ((::TSCNParameter*)p);
   }
   static void deleteArray_TSCNParameter(void *p) {
      delete [] ((::TSCNParameter*)p);
   }
   static void destruct_TSCNParameter(void *p) {
      typedef ::TSCNParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCNParameter

//______________________________________________________________________________
void TXRSParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TXRSParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TXRSParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TXRSParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TXRSParameter(void *p) {
      return  p ? new(p) ::TXRSParameter : new ::TXRSParameter;
   }
   static void *newArray_TXRSParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TXRSParameter[nElements] : new ::TXRSParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TXRSParameter(void *p) {
      delete ((::TXRSParameter*)p);
   }
   static void deleteArray_TXRSParameter(void *p) {
      delete [] ((::TXRSParameter*)p);
   }
   static void destruct_TXRSParameter(void *p) {
      typedef ::TXRSParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TXRSParameter

//______________________________________________________________________________
void Map1::Streamer(TBuffer &R__b)
{
   // Stream an object of class Map1.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Map1::Class(),this);
   } else {
      R__b.WriteClassBuffer(Map1::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Map1(void *p) {
      return  p ? new(p) ::Map1 : new ::Map1;
   }
   static void *newArray_Map1(Long_t nElements, void *p) {
      return p ? new(p) ::Map1[nElements] : new ::Map1[nElements];
   }
   // Wrapper around operator delete
   static void delete_Map1(void *p) {
      delete ((::Map1*)p);
   }
   static void deleteArray_Map1(void *p) {
      delete [] ((::Map1*)p);
   }
   static void destruct_Map1(void *p) {
      typedef ::Map1 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Map1

//______________________________________________________________________________
void TModParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TModParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TModParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TModParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TModParameter(void *p) {
      return  p ? new(p) ::TModParameter : new ::TModParameter;
   }
   static void *newArray_TModParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TModParameter[nElements] : new ::TModParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TModParameter(void *p) {
      delete ((::TModParameter*)p);
   }
   static void deleteArray_TModParameter(void *p) {
      delete [] ((::TModParameter*)p);
   }
   static void destruct_TModParameter(void *p) {
      typedef ::TModParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TModParameter

//______________________________________________________________________________
void TMWParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMWParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TMWParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TMWParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TMWParameter(void *p) {
      return  p ? new(p) ::TMWParameter : new ::TMWParameter;
   }
   static void *newArray_TMWParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TMWParameter[nElements] : new ::TMWParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TMWParameter(void *p) {
      delete ((::TMWParameter*)p);
   }
   static void deleteArray_TMWParameter(void *p) {
      delete [] ((::TMWParameter*)p);
   }
   static void destruct_TMWParameter(void *p) {
      typedef ::TMWParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TMWParameter

//______________________________________________________________________________
void TTPCParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TTPCParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TTPCParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TTPCParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TTPCParameter(void *p) {
      return  p ? new(p) ::TTPCParameter : new ::TTPCParameter;
   }
   static void *newArray_TTPCParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TTPCParameter[nElements] : new ::TTPCParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TTPCParameter(void *p) {
      delete ((::TTPCParameter*)p);
   }
   static void deleteArray_TTPCParameter(void *p) {
      delete [] ((::TTPCParameter*)p);
   }
   static void destruct_TTPCParameter(void *p) {
      typedef ::TTPCParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TTPCParameter

//______________________________________________________________________________
void TMUSICParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMUSICParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TMUSICParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TMUSICParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TMUSICParameter(void *p) {
      return  p ? new(p) ::TMUSICParameter : new ::TMUSICParameter;
   }
   static void *newArray_TMUSICParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TMUSICParameter[nElements] : new ::TMUSICParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TMUSICParameter(void *p) {
      delete ((::TMUSICParameter*)p);
   }
   static void deleteArray_TMUSICParameter(void *p) {
      delete [] ((::TMUSICParameter*)p);
   }
   static void destruct_TMUSICParameter(void *p) {
      typedef ::TMUSICParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TMUSICParameter

//______________________________________________________________________________
void TSCIParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCIParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCIParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCIParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCIParameter(void *p) {
      return  p ? new(p) ::TSCIParameter : new ::TSCIParameter;
   }
   static void *newArray_TSCIParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TSCIParameter[nElements] : new ::TSCIParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCIParameter(void *p) {
      delete ((::TSCIParameter*)p);
   }
   static void deleteArray_TSCIParameter(void *p) {
      delete [] ((::TSCIParameter*)p);
   }
   static void destruct_TSCIParameter(void *p) {
      typedef ::TSCIParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCIParameter

//______________________________________________________________________________
void TIDParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TIDParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TIDParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TIDParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TIDParameter(void *p) {
      return  p ? new(p) ::TIDParameter : new ::TIDParameter;
   }
   static void *newArray_TIDParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TIDParameter[nElements] : new ::TIDParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TIDParameter(void *p) {
      delete ((::TIDParameter*)p);
   }
   static void deleteArray_TIDParameter(void *p) {
      delete [] ((::TIDParameter*)p);
   }
   static void destruct_TIDParameter(void *p) {
      typedef ::TIDParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TIDParameter

//______________________________________________________________________________
void TSIParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSIParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSIParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSIParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSIParameter(void *p) {
      return  p ? new(p) ::TSIParameter : new ::TSIParameter;
   }
   static void *newArray_TSIParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TSIParameter[nElements] : new ::TSIParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSIParameter(void *p) {
      delete ((::TSIParameter*)p);
   }
   static void deleteArray_TSIParameter(void *p) {
      delete [] ((::TSIParameter*)p);
   }
   static void destruct_TSIParameter(void *p) {
      typedef ::TSIParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSIParameter

//______________________________________________________________________________
void TMRTOFMSParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMRTOFMSParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TMRTOFMSParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(TMRTOFMSParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TMRTOFMSParameter(void *p) {
      return  p ? new(p) ::TMRTOFMSParameter : new ::TMRTOFMSParameter;
   }
   static void *newArray_TMRTOFMSParameter(Long_t nElements, void *p) {
      return p ? new(p) ::TMRTOFMSParameter[nElements] : new ::TMRTOFMSParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_TMRTOFMSParameter(void *p) {
      delete ((::TMRTOFMSParameter*)p);
   }
   static void deleteArray_TMRTOFMSParameter(void *p) {
      delete [] ((::TMRTOFMSParameter*)p);
   }
   static void destruct_TMRTOFMSParameter(void *p) {
      typedef ::TMRTOFMSParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TMRTOFMSParameter

//______________________________________________________________________________
void TSCNUnpackEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCNUnpackEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCNUnpackEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCNUnpackEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCNUnpackEvent(void *p) {
      return  p ? new(p) ::TSCNUnpackEvent : new ::TSCNUnpackEvent;
   }
   static void *newArray_TSCNUnpackEvent(Long_t nElements, void *p) {
      return p ? new(p) ::TSCNUnpackEvent[nElements] : new ::TSCNUnpackEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCNUnpackEvent(void *p) {
      delete ((::TSCNUnpackEvent*)p);
   }
   static void deleteArray_TSCNUnpackEvent(void *p) {
      delete [] ((::TSCNUnpackEvent*)p);
   }
   static void destruct_TSCNUnpackEvent(void *p) {
      typedef ::TSCNUnpackEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCNUnpackEvent

//______________________________________________________________________________
void TSCNCalProc::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCNCalProc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCNCalProc::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCNCalProc::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCNCalProc(void *p) {
      return  p ? new(p) ::TSCNCalProc : new ::TSCNCalProc;
   }
   static void *newArray_TSCNCalProc(Long_t nElements, void *p) {
      return p ? new(p) ::TSCNCalProc[nElements] : new ::TSCNCalProc[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCNCalProc(void *p) {
      delete ((::TSCNCalProc*)p);
   }
   static void deleteArray_TSCNCalProc(void *p) {
      delete [] ((::TSCNCalProc*)p);
   }
   static void destruct_TSCNCalProc(void *p) {
      typedef ::TSCNCalProc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCNCalProc

//______________________________________________________________________________
void TSCNCalEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCNCalEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCNCalEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCNCalEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCNCalEvent(void *p) {
      return  p ? new(p) ::TSCNCalEvent : new ::TSCNCalEvent;
   }
   static void *newArray_TSCNCalEvent(Long_t nElements, void *p) {
      return p ? new(p) ::TSCNCalEvent[nElements] : new ::TSCNCalEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCNCalEvent(void *p) {
      delete ((::TSCNCalEvent*)p);
   }
   static void deleteArray_TSCNCalEvent(void *p) {
      delete [] ((::TSCNCalEvent*)p);
   }
   static void destruct_TSCNCalEvent(void *p) {
      typedef ::TSCNCalEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCNCalEvent

//______________________________________________________________________________
void TSCNAnlEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCNAnlEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCNAnlEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCNAnlEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCNAnlEvent(void *p) {
      return  p ? new(p) ::TSCNAnlEvent : new ::TSCNAnlEvent;
   }
   static void *newArray_TSCNAnlEvent(Long_t nElements, void *p) {
      return p ? new(p) ::TSCNAnlEvent[nElements] : new ::TSCNAnlEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCNAnlEvent(void *p) {
      delete ((::TSCNAnlEvent*)p);
   }
   static void deleteArray_TSCNAnlEvent(void *p) {
      delete [] ((::TSCNAnlEvent*)p);
   }
   static void destruct_TSCNAnlEvent(void *p) {
      typedef ::TSCNAnlEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCNAnlEvent

//______________________________________________________________________________
void TSCNUnpackProc::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCNUnpackProc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCNUnpackProc::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCNUnpackProc::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCNUnpackProc(void *p) {
      return  p ? new(p) ::TSCNUnpackProc : new ::TSCNUnpackProc;
   }
   static void *newArray_TSCNUnpackProc(Long_t nElements, void *p) {
      return p ? new(p) ::TSCNUnpackProc[nElements] : new ::TSCNUnpackProc[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCNUnpackProc(void *p) {
      delete ((::TSCNUnpackProc*)p);
   }
   static void deleteArray_TSCNUnpackProc(void *p) {
      delete [] ((::TSCNUnpackProc*)p);
   }
   static void destruct_TSCNUnpackProc(void *p) {
      typedef ::TSCNUnpackProc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCNUnpackProc

//______________________________________________________________________________
void TSCNAnlProc::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCNAnlProc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCNAnlProc::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCNAnlProc::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCNAnlProc(void *p) {
      return  p ? new(p) ::TSCNAnlProc : new ::TSCNAnlProc;
   }
   static void *newArray_TSCNAnlProc(Long_t nElements, void *p) {
      return p ? new(p) ::TSCNAnlProc[nElements] : new ::TSCNAnlProc[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCNAnlProc(void *p) {
      delete ((::TSCNAnlProc*)p);
   }
   static void deleteArray_TSCNAnlProc(void *p) {
      delete [] ((::TSCNAnlProc*)p);
   }
   static void destruct_TSCNAnlProc(void *p) {
      typedef ::TSCNAnlProc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCNAnlProc

//______________________________________________________________________________
void TSCNAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSCNAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TSCNAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(TSCNAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSCNAnalysis(void *p) {
      return  p ? new(p) ::TSCNAnalysis : new ::TSCNAnalysis;
   }
   static void *newArray_TSCNAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::TSCNAnalysis[nElements] : new ::TSCNAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSCNAnalysis(void *p) {
      delete ((::TSCNAnalysis*)p);
   }
   static void deleteArray_TSCNAnalysis(void *p) {
      delete [] ((::TSCNAnalysis*)p);
   }
   static void destruct_TSCNAnalysis(void *p) {
      typedef ::TSCNAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TSCNAnalysis

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 214,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 214,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *maplEintcOmaplEintcOintgRsPgR_Dictionary();
   static void maplEintcOmaplEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOmaplEintcOintgRsPgR(void *p = 0);
   static void *newArray_maplEintcOmaplEintcOintgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOmaplEintcOintgRsPgR(void *p);
   static void deleteArray_maplEintcOmaplEintcOintgRsPgR(void *p);
   static void destruct_maplEintcOmaplEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,map<int,int> >*)
   {
      map<int,map<int,int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,map<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,map<int,int> >", -2, "map", 96,
                  typeid(map<int,map<int,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOmaplEintcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,map<int,int> >) );
      instance.SetNew(&new_maplEintcOmaplEintcOintgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOmaplEintcOintgRsPgR);
      instance.SetDelete(&delete_maplEintcOmaplEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOmaplEintcOintgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOmaplEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,map<int,int> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,map<int,int> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOmaplEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,map<int,int> >*)0x0)->GetClass();
      maplEintcOmaplEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOmaplEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOmaplEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,int> > : new map<int,map<int,int> >;
   }
   static void *newArray_maplEintcOmaplEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,int> >[nElements] : new map<int,map<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOmaplEintcOintgRsPgR(void *p) {
      delete ((map<int,map<int,int> >*)p);
   }
   static void deleteArray_maplEintcOmaplEintcOintgRsPgR(void *p) {
      delete [] ((map<int,map<int,int> >*)p);
   }
   static void destruct_maplEintcOmaplEintcOintgRsPgR(void *p) {
      typedef map<int,map<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,map<int,int> >

namespace ROOT {
   static TClass *maplEintcOintgR_Dictionary();
   static void maplEintcOintgR_TClassManip(TClass*);
   static void *new_maplEintcOintgR(void *p = 0);
   static void *newArray_maplEintcOintgR(Long_t size, void *p);
   static void delete_maplEintcOintgR(void *p);
   static void deleteArray_maplEintcOintgR(void *p);
   static void destruct_maplEintcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,int>*)
   {
      map<int,int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,int>", -2, "map", 96,
                  typeid(map<int,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,int>) );
      instance.SetNew(&new_maplEintcOintgR);
      instance.SetNewArray(&newArray_maplEintcOintgR);
      instance.SetDelete(&delete_maplEintcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOintgR);
      instance.SetDestructor(&destruct_maplEintcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,int>*)0x0)->GetClass();
      maplEintcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int> : new map<int,int>;
   }
   static void *newArray_maplEintcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int>[nElements] : new map<int,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOintgR(void *p) {
      delete ((map<int,int>*)p);
   }
   static void deleteArray_maplEintcOintgR(void *p) {
      delete [] ((map<int,int>*)p);
   }
   static void destruct_maplEintcOintgR(void *p) {
      typedef map<int,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,int>

namespace {
  void TriggerDictionaryInitialization_libGo4UserAnalysis_Impl() {
    static const char* headers[] = {
"./AIDA_Headers.h",
"./TSCNParameter.h",
"./FATIMA_Gain_Match.h",
"./PLASTIC_Data_Stream.h",
"./WR_Structure.h",
"./Events.h",
"./Gap_Handler.h",
"./PLASTIC_Calibrator.h",
"./FRS_Detector_System.h",
"./FINGER_Data_Stream.h",
"./FATIMA_Time_Calibration.h",
"./Raw_Event.h",
"./FINGER_Detector_System.h",
"./TSCNCalProc.h",
"./TXRSParameter.h",
"./FINGER_Calibrator.h",
"./Match.h",
"./TAMEX.h",
"./TDC_1290.h",
"./Time_EventBuilder.h",
"./GALILEO_Time_Calibration.h",
"./Beta_Event.h",
"./TSCNAnlEvent.h",
"./First_Test.h",
"./AIDA_Data_Types.h",
"./FATIMA_Detector_System.h",
"./FATIMA_Energy_Calibration.h",
"./Raw_Event_Storage.h",
"./TSCNUnpackEvent.h",
"./Event_Store.h",
"./FEBEX.h",
"./PLASTIC_Detector_System.h",
"./QDC_751.h",
"./TSCNAnlProc.h",
"./FATIMA_Event.h",
"./GALILEO_Energy_Calibration.h",
"./AIDA_Detector_System.h",
"./PLASTIC_Event.h",
"./White_Rabbit.h",
"./GALILEO_Detector_System_TEST.h",
"./TSCNCalEvent.h",
"./TSCNAnalysis.h",
"./GALILEO_Event.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/csee.gsi.de/go4/503-00/include",
"/cvmfs/csee.gsi.de/go4/503-00",
"/cvmfs/csee.gsi.de/root/612-06/include",
"/u/pnapiral/test_Beta/DESPEC-Analysis-Framework/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libGo4UserAnalysis dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$./TSCNParameter.h")))  TSCNParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TXRSParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  Map1;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TModParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TMWParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TTPCParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TMUSICParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TSCIParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TIDParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TSIParameter;
class __attribute__((annotate("$clingAutoload$TXRSParameter.h")))  __attribute__((annotate("$clingAutoload$./FRS_Detector_System.h")))  TMRTOFMSParameter;
class __attribute__((annotate("$clingAutoload$TSCNUnpackEvent.h")))  __attribute__((annotate("$clingAutoload$./TSCNCalProc.h")))  TSCNUnpackEvent;
class __attribute__((annotate("$clingAutoload$./TSCNCalProc.h")))  TSCNCalProc;
class __attribute__((annotate("$clingAutoload$TSCNCalEvent.h")))  __attribute__((annotate("$clingAutoload$./TSCNAnlEvent.h")))  TSCNCalEvent;
class __attribute__((annotate("$clingAutoload$./TSCNAnlEvent.h")))  TSCNAnlEvent;
class __attribute__((annotate("$clingAutoload$./First_Test.h")))  TSCNUnpackProc;
class __attribute__((annotate("$clingAutoload$./TSCNAnlProc.h")))  TSCNAnlProc;
class __attribute__((annotate("$clingAutoload$./TSCNAnalysis.h")))  TSCNAnalysis;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libGo4UserAnalysis dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef Linux
  #define Linux 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "./AIDA_Headers.h"
#include "./TSCNParameter.h"
#include "./FATIMA_Gain_Match.h"
#include "./PLASTIC_Data_Stream.h"
#include "./WR_Structure.h"
#include "./Events.h"
#include "./Gap_Handler.h"
#include "./PLASTIC_Calibrator.h"
#include "./FRS_Detector_System.h"
#include "./FINGER_Data_Stream.h"
#include "./FATIMA_Time_Calibration.h"
#include "./Raw_Event.h"
#include "./FINGER_Detector_System.h"
#include "./TSCNCalProc.h"
#include "./TXRSParameter.h"
#include "./FINGER_Calibrator.h"
#include "./Match.h"
#include "./TAMEX.h"
#include "./TDC_1290.h"
#include "./Time_EventBuilder.h"
#include "./GALILEO_Time_Calibration.h"
#include "./Beta_Event.h"
#include "./TSCNAnlEvent.h"
#include "./First_Test.h"
#include "./AIDA_Data_Types.h"
#include "./FATIMA_Detector_System.h"
#include "./FATIMA_Energy_Calibration.h"
#include "./Raw_Event_Storage.h"
#include "./TSCNUnpackEvent.h"
#include "./Event_Store.h"
#include "./FEBEX.h"
#include "./PLASTIC_Detector_System.h"
#include "./QDC_751.h"
#include "./TSCNAnlProc.h"
#include "./FATIMA_Event.h"
#include "./GALILEO_Energy_Calibration.h"
#include "./AIDA_Detector_System.h"
#include "./PLASTIC_Event.h"
#include "./White_Rabbit.h"
#include "./GALILEO_Detector_System_TEST.h"
#include "./TSCNCalEvent.h"
#include "./TSCNAnalysis.h"
#include "./GALILEO_Event.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Map1", payloadCode, "@",
"TIDParameter", payloadCode, "@",
"TMRTOFMSParameter", payloadCode, "@",
"TMUSICParameter", payloadCode, "@",
"TMWParameter", payloadCode, "@",
"TModParameter", payloadCode, "@",
"TSCIParameter", payloadCode, "@",
"TSCNAnalysis", payloadCode, "@",
"TSCNAnlEvent", payloadCode, "@",
"TSCNAnlProc", payloadCode, "@",
"TSCNCalEvent", payloadCode, "@",
"TSCNCalProc", payloadCode, "@",
"TSCNParameter", payloadCode, "@",
"TSCNUnpackEvent", payloadCode, "@",
"TSCNUnpackProc", payloadCode, "@",
"TSIParameter", payloadCode, "@",
"TTPCParameter", payloadCode, "@",
"TXRSParameter", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libGo4UserAnalysis",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libGo4UserAnalysis_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libGo4UserAnalysis_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libGo4UserAnalysis() {
  TriggerDictionaryInitialization_libGo4UserAnalysis_Impl();
}
