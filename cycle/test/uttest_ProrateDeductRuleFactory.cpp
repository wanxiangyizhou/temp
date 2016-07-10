#include <unittest_prorateDeductRule_main.h>
#include "../ProrateDeductRule.h"
#include "../IProrateDeductRule.h"
#include "ProrateDeductRuleFactory.h"
#include <utest_common.h>
using namespace rating_charging;
using namespace charge_utest;

SUITE(SUITE_ProrateDeductRuleFactory_cpp)
{
	//Fuction: CProrateDeductRuleFactory :: ~ CProrateDeductRuleFactory ( void )
	TEST_FIXTURE(TestFixture,TestCProrateDeductRuleFactory)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: BEGIN_NAMESPACE CProrateDeductRuleFactory :: CProrateDeductRuleFactory ( void )
	TEST_FIXTURE(TestFixture,TestCProrateDeductRuleFactory_CProrateDeductRuleFactory)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: void CProrateDeductRuleFactory :: clear ( )
	TEST_FIXTURE(TestFixture,TestCProrateDeductRuleFactory_clear)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const IProrateDeductRule & CProrateDeductRuleFactory :: get_prorateDeductRuleInterface ( ) const
	TEST_FIXTURE(TestFixture,TestCProrateDeductRuleFactory_get_prorateDeductRuleInterface)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: void CProrateDeductRuleFactory :: init ( const xc :: CSnapshot & cSnapShot )
	TEST_FIXTURE(TestFixture,TestCProrateDeductRuleFactory_init)
	{
		/*codes for test start here*/
		try
		{
			AISTD auto_ptr<xc::CSnapshot> autoPtrSnapshort=getSnap();
			CHECK(NULL!=autoPtrSnapshort.get());
			CProrateDeductRuleFactory *pProrateDeductRuleFactory=new CProrateDeductRuleFactory();
			
			const IProrateDeductRule& prorateDeductRule=pProrateDeductRuleFactory->get_prorateDeductRuleInterface();
			
	        int32 offer_prod_id = 88880005;
	        int32 bill_type = 0;
	        int32 main_promotion = 100;
	        SProrateRule prorate_rule;
	        
	        prorateDeductRule.get_accumulateProrateRule(offer_prod_id,bill_type,main_promotion,prorate_rule);	        
	        CHECK_EQUAL(0,prorate_rule.prorate_type_first);
	        CHECK_EQUAL(0,prorate_rule.prorate_type_last);
	        
	        int32 iRet=-1;
	         try
	         {
           		iRet=prorateDeductRule.get_deductFlag(offer_prod_id,bill_type,main_promotion);
       		}
			catch (const CExpX& exp)
			{
				RATING_TRACES("error_code: %d, msg: %s",exp.get_errno(),exp.what());
				CHECK_EQUAL(622039,exp.get_errno());
			}
			
	        try
	        {
	            int32 iRet = prorateDeductRule.get_resourceFlag(offer_prod_id,bill_type,main_promotion);
	        }
			catch (const CExpX& exp)
			{
				RATING_TRACES("error_code: %d, msg: %s",exp.get_errno(),exp.what());
				CHECK_EQUAL(622039,exp.get_errno());
			}		
				
			offer_prod_id = 88880004;
			prorateDeductRule.get_freeResProrateRule(offer_prod_id,bill_type,main_promotion,prorate_rule);
	        CHECK_EQUAL(0,prorate_rule.prorate_type_first);
	        CHECK_EQUAL(0,prorate_rule.prorate_type_last);
/////////////////////////////////////////////////////////////////////////////	        
	        pProrateDeductRuleFactory->init(*(autoPtrSnapshort.get()));
/////////////////////////////////////////////////////////////////////////////

	        offer_prod_id = 88880005;
	        prorateDeductRule.get_accumulateProrateRule(offer_prod_id,bill_type,main_promotion,prorate_rule);	        
	        RATING_TRACES("prorate_rule: prorate_type_first[%d], prorate_type_last[%d]",prorate_rule.prorate_type_first,prorate_rule.prorate_type_last);
	        CHECK_EQUAL(1,prorate_rule.prorate_type_first);
	        CHECK_EQUAL(0,prorate_rule.prorate_type_last);
			
			iRet=-1;
			iRet= prorateDeductRule.get_deductFlag(offer_prod_id,bill_type,main_promotion);
        		CHECK_EQUAL(0,iRet);
   			
   			iRet=-1;
   			iRet = prorateDeductRule.get_resourceFlag(offer_prod_id,bill_type,main_promotion);
        		CHECK_EQUAL(0,iRet);
        		
        		offer_prod_id = 88880004;
           	prorateDeductRule.get_freeResProrateRule(offer_prod_id,bill_type,main_promotion,prorate_rule);   
       		RATING_TRACES("prorate_rule: prorate_type_first[%d], prorate_type_last[%d]",prorate_rule.prorate_type_first,prorate_rule.prorate_type_last);
	        CHECK_EQUAL(1,prorate_rule.prorate_type_first);
	        CHECK_EQUAL(0,prorate_rule.prorate_type_last);
	        		
			pProrateDeductRuleFactory->clear();
	   	}
   		catch (const CExpX& exp)
   		{
   			RATING_TRACES("error_code: %d, msg: %s",exp.get_errno(),exp.what());
   		}

		/*codes for test stop here*/
	}
}
