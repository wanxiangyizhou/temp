#include <unittest_prodBillCycle_main.h>
#include "../ProdBillCycle.h"
#include "../IProrateDeductRule.h"
#include <utest_common.h>
using namespace rating_charging;
using namespace charge_utest;

SUITE(SUITE_ProdBillCycle_cpp)
{
	//Fuction: CAccountCycle ( )
	TEST_FIXTURE(TestFixture,TestCAccountCycle_CAccountCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CAccountCycle :: CAccountCycle ( const int64 doneCode , const MXdr :: SAcctBillCycle & acctBillCycle , const MXdr :: SRateProdId & prodOrderInfo , const int64 llProcessDate , const SProrateRule & prorate_rules ) : m_llProcessDate ( llProcessDate ) , CBillCycle ( getICleanCycle ( ) , acctBillCycle . get_firstBillDate ( ) , acctBillCycle . get_billValidDate ( ) , acctBillCycle . get_billExpireDate ( ) ) , CProdBillCycleImpl ( doneCode , prodOrderInfo , CleanCycleFactory :: toEnumCycleType ( acctBillCycle . get_cycleType ( ) ) , acctBillCycle . get_cycleUnit ( ) , prorate_rules )
	TEST_FIXTURE(TestFixture,TestCAccountCycle_CAccountCycle_CAccountCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: void CAccountCycle :: cal_validAndExpireDate ( int64 & llRealValidDate , int64 & llRealExpireDate , int64 & llHopeValidDate , int64 & llHopeExpireDate , const bool IsRefProdExpireDate , const bool IsRefProdValidDate ) const
	TEST_FIXTURE(TestFixture,TestCAccountCycle_CAccountCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const int32 CAccountCycle :: get_cycleNum ( ) const
	TEST_FIXTURE(TestFixture,TestCAccountCycle_CAccountCycle_get_cycleNum)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CBillCycle ( )
	TEST_FIXTURE(TestFixture,TestCBillCycle_CBillCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CBillCycle :: CBillCycle ( const ICleanCycle * pCycle , const int64 firstExpireDate , const int64 cycleValidDate , const int64 cycleExpireDate ) : m_cycleValidDate ( cycleValidDate ) , m_cycleExpireDate ( cycleExpireDate ) , m_firstExpireDate ( firstExpireDate ) , m_bIsHaveFirstExpireDate ( firstExpireDate >= cycleValidDate && firstExpireDate < cycleExpireDate ) , m_pCycle ( pCycle )
	TEST_FIXTURE(TestFixture,TestCBillCycle_CBillCycle_CBillCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const int32 CBillCycle :: cal_validAndExpireDate ( const int64 llProcessDate , int64 & llRealValidDate , int64 & llRealExpireDate , int64 & llHopeValidDate , int64 & llHopeExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestCBillCycle_CBillCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 CBillCycleFactory :: cal_acctBillCycle ( const MXdr :: SAcctBillCycle & acctBillCycle , const int64 processTime , int64 & llValidDate , int64 & llExpireDate )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_cal_acctBillCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int64 CBillCycleFactory :: cal_expireDate ( const int64 processTime , const MXdr :: SSubCommon & subCommon , const int64 doneCode , const int32 cycleRefMode , const CycleType cycleType , const int32 cycleUnit )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_cal_expireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const MXdr :: SAcctBillCycle & CBillCycleFactory :: checkAcctBillCycle ( const int64 doneCode , const int64 processTime , const MXdr :: SSubCommon & subCommon )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_checkAcctBillCycle)
	{
		/*codes for test start here*/
		MXdr::SAcctBillCycle acctBillCycle;
		CBSDateTime processTime ( "20131203110101" );
		int64 doneCode = 21312ll;
		MXdr::SSubCommon subCommon;
		
		try
		{
    		subCommon.userBaseInfo().set_acctBillCycle(acctBillCycle);
    		CBillCycleFactory::checkAcctBillCycle(doneCode,processTime,subCommon);
        }
		catch (const CExpX& exp)
		{
			CHECK_EQUAL(exp.get_errno(),622099);
		}
		
		acctBillCycle.set_billValidDate(toDateTime_withoutCheck(20121203110101));
		acctBillCycle.set_billExpireDate(toDateTime_withoutCheck(20141203110101));
		
		try
		{
    		subCommon.userBaseInfo().set_acctBillCycle(acctBillCycle);
    		CBillCycleFactory::checkAcctBillCycle(doneCode,processTime,subCommon);
        }
		catch (const CExpX& exp)
		{
			CHECK_EQUAL(exp.get_errno(),622099);
		}
		
		acctBillCycle.cycleType() = 1;
		subCommon.userBaseInfo().set_acctBillCycle(acctBillCycle);
		CBillCycleFactory::checkAcctBillCycle(acctBillCycle,processTime);
		
		
		/*codes for test stop here*/
	}
	//Fuction: int64 CBillCycleFactory :: checkActivation ( const MXdr :: SRateProdId & rateProdInfo , const int64 doneCode , const int64 processTime , const MXdr :: SSubCommon & subCommon )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_checkActivation)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const MXdr :: SRateProdId & CBillCycleFactory :: checkProd ( const int64 doneCode , const int64 processTime , const MXdr :: SSubCommon & subCommon )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_checkProd)
	{
	    #define TestCBillCycleFactory_checkProd_CHECK_EQUAL(value) \
		try\
		{\
            CBillCycleFactory::checkProd(doneCode,processTime,eProdCycle,subCommon.get_rateProdId(),llDoneCode,rateProdId);\
        }\
		catch (const CExpX& exp)\
		{\
			CHECK_EQUAL(exp.get_errno(),value);\
		}
	    
		/*codes for test start here*/
		CBSDateTime processTime ("20131203110101");
		int64 doneCode = 21312ll;
		MXdr::SSubCommon subCommon;
		MXdr::SRateProdId rateProdId;
		int64 llDoneCode=0;
		TestCBillCycleFactory_checkProd_CHECK_EQUAL(622019);
		
		
		subCommon.rateProdId().insert(make_pair(21312ll,rateProdId));
		TestCBillCycleFactory_checkProd_CHECK_EQUAL(622020);
		
		subCommon.rateProdId().clear();
		rateProdId.validDateTime() = toDateTime_withoutCheck(20121203110101);
		rateProdId.expireDateTime() = toDateTime_withoutCheck(20141203110101);
		subCommon.rateProdId().insert(make_pair(21312ll,rateProdId));
		TestCBillCycleFactory_checkProd_CHECK_EQUAL(0);

		/*codes for test stop here*/
	}
	//Fuction: const MXdr :: SRateProdBillCycle & CBillCycleFactory :: checkProdBillCycle ( const MXdr :: SRateProdId & rateProdInfo , const int64 doneCode , const int64 processTime , const MXdr :: SSubCommon & subCommon )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_checkProdBillCycle)
	{
		/*codes for test start here*/
	    #define TestCBillCycleFactory_checkProdBillCycle_CHECK_EQUAL(value) \
		try\
		{\
            CBillCycleFactory::checkProdBillCycle(rateProdId, doneCode, processTime, subCommon);\
        }\
		catch (const CExpX& exp)\
		{\
			CHECK_EQUAL(exp.get_errno(),value);\
		}
		
		CBSDateTime processTime ( "20131203110101");
		int64 doneCode = 21312ll;
		MXdr::SRateProdId rateProdId;
		MXdr::SSubCommon subCommon;
        
        TestCBillCycleFactory_checkProdBillCycle_CHECK_EQUAL(622031);
		
		MXdr::SRateProdBillCycle rateProdBillCycle;
		subCommon.rateProdBillCycle().insert(make_pair(21312ll,rateProdBillCycle));
		TestCBillCycleFactory_checkProdBillCycle_CHECK_EQUAL(622032);

		/*codes for test stop here*/
	}
	//Fuction: int64 CBillCycleFactory :: checkUserAccess ( const MXdr :: SRateProdId & rateProdInfo , const int64 doneCode , const int64 processTime , const MXdr :: SSubCommon & subCommon )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_checkUserAccess)
	{
		/*codes for test start here*/
	    #define TestCBillCycleFactory_checkUserAccess_CHECK_EQUAL(value) \
		try\
		{\
            CBillCycleFactory::checkUserAccess(rateProdId, doneCode, processTime, subCommon);\
        }\
		catch (const CExpX& exp)\
		{\
			CHECK_EQUAL(exp.get_errno(),value);\
		}
		
		CBSDateTime processTime ( "20131203110101");
		int64 doneCode = 21312ll;
		MXdr::SRateProdId rateProdId;
		MXdr::SSubCommon subCommon;
        
        TestCBillCycleFactory_checkUserAccess_CHECK_EQUAL(622025);
        
        subCommon.userBaseInfo().createDate() = toDateTime_withoutCheck(20141203110101);
        TestCBillCycleFactory_checkUserAccess_CHECK_EQUAL(622026);
        
        
        subCommon.userBaseInfo().createDate() = toDateTime_withoutCheck(20121203110101);
        rateProdId.validDateTime() = toDateTime_withoutCheck(20111203110101);
        TestCBillCycleFactory_checkUserAccess_CHECK_EQUAL(622027);
        
        
        subCommon.userBaseInfo().createDate() = toDateTime_withoutCheck(20121203110101);
        rateProdId.validDateTime() = toDateTime_withoutCheck(20131203110101);
        TestCBillCycleFactory_checkUserAccess_CHECK_EQUAL(0);

		/*codes for test stop here*/
	}
	//Fuction: const CProdBillCycle * CBillCycleFactory :: get_billCycleWithConfig ( const int64 doneCode , const int64 processTime , const MXdr :: SSubCommon & subCommon , const int32 cycleRefMode , const CycleType cycleType , const int32 cycleUnit )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_get_billCycleWithConfig)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const CProdBillCycle * CBillCycleFactory :: get_billCycleWithDiscountAndConfig ( const int64 doneCode , const int64 processTime , const MXdr :: SSubCommon & subCommon , const int32 cycleRefMode , const CycleType cycleType , const int32 cycleUnit , const SProrateRule & prorate_rules )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_get_billCycleWithDiscountAndConfig)
	{
		/*codes for test start here*/
		
		CBSDateTime processTime (" 20131203110101");
		int64 doneCode = 21312ll;
		
		MXdr::SSubCommon subCommon;
		MXdr::SAcctBillCycle acctBillCycle;
		acctBillCycle.set_billValidDate(toDateTime_withoutCheck(20121203110101));
		acctBillCycle.set_billExpireDate(toDateTime_withoutCheck(20141203110101));
		acctBillCycle.cycleType() = 1;
		subCommon.userBaseInfo().set_acctBillCycle(acctBillCycle);
		
		
		MXdr::SRateProdId rateProdId;
		rateProdId.validDateTime() = toDateTime_withoutCheck(20121203110101);
		rateProdId.expireDateTime() = toDateTime_withoutCheck(20141203110101);
		subCommon.rateProdId().insert(make_pair(21312ll,rateProdId));
		
		
		int32 cycleRefMode = 10;
		CycleType cycleType;
		int32 cycleUnit;
		SProrateRule prorate_rules;
		try{
            CBillCycleFactory::get_billCycleWithDiscountAndConfig(doneCode,processTime,subCommon,cycleRefMode,cycleType,cycleUnit,prorate_rules);
        }
        catch (const CExpX& exp)
		{
			CHECK_EQUAL(exp.get_errno(),622037);
		}

		/*codes for test stop here*/
	}
	//Fuction: const CProdBillCycle * CBillCycleFactory :: get_billCycleWithoutConfig ( const int64 doneCode , const int64 processTime , const MXdr :: SSubCommon & subCommon , const int32 cycleRefMode )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_get_billCycleWithoutConfig)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: } const RefModeType CBillCycleFactory :: toEnumRefModeType ( const int32 cycleRefMode )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_toEnumRefModeType)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CConfigCycle ( )
	TEST_FIXTURE(TestFixture,TestCConfigCycle_CConfigCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CConfigCycle :: CConfigCycle ( const int64 doneCode , const CycleType cycleType , const int32 cycleUnit , const SProrateRule & prorate_rules , const MXdr :: SRateProdId & prodOrderInfo , const int64 llProcessDate , const int64 llBaseDate ) : m_llBaseDate ( llBaseDate ) , m_llProcessDate ( llProcessDate ) , CProdBillCycleImpl ( doneCode , prodOrderInfo , cycleType , cycleUnit , prorate_rules )
	TEST_FIXTURE(TestFixture,TestCConfigCycle_CConfigCycle_CConfigCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: void CConfigCycle :: cal_validAndExpireDate ( int64 & llRealValidDate , int64 & llRealExpireDate , int64 & llHopeValidDate , int64 & llHopeExpireDate , const bool IsRefProdExpireDate , const bool IsRefProdValidDate ) const
	TEST_FIXTURE(TestFixture,TestCConfigCycle_CConfigCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CConfigCycleEx ( )
	TEST_FIXTURE(TestFixture,TestCConfigCycleEx_CConfigCycleEx)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CConfigCycleEx :: CConfigCycleEx ( const int64 doneCode , const CycleType cycleType , const int32 cycleUnit , const SProrateRule & prorate_rules , const MXdr :: SRateProdId & prodOrderInfo , const int64 llProcessDate , const int64 llBaseDate ) : m_llBaseDate ( llBaseDate ) , _configCycle ( new CConfigCycle ( doneCode , cycleType , cycleUnit , prorate_rules , prodOrderInfo , llProcessDate , llBaseDate ) )
	TEST_FIXTURE(TestFixture,TestCConfigCycleEx_CConfigCycleEx_CConfigCycleEx)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: void CConfigCycleEx :: cal_validAndExpireDate ( int64 & llRealValidDate , int64 & llRealExpireDate , int64 & llHopeValidDate , int64 & llHopeExpireDate , const bool IsRefProdExpireDate , const bool IsRefProdValidDate ) const
	TEST_FIXTURE(TestFixture,TestCConfigCycleEx_CConfigCycleEx_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const int32 CConfigCycleEx :: get_cycleNum ( ) const
	TEST_FIXTURE(TestFixture,TestCConfigCycleEx_CConfigCycleEx_get_cycleNum)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float CConfigCycleEx :: get_discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestCConfigCycleEx_CConfigCycleEx_get_discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CProdBillCycleImpl :: ~ CProdBillCycleImpl ( )
	TEST_FIXTURE(TestFixture,TestCProdBillCycleImpl_CProdBillCycleImpl)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CProdBillCycleImpl :: CProdBillCycleImpl ( const int64 doneCode , const MXdr :: SRateProdId & prodOrderInfo , const CycleType cycleType , const int32 nCycleUnit , const SProrateRule & prorate_rules ) : m_llDoneCode ( doneCode ) , m_ProdValidDate ( prodOrderInfo . get_validDateTime ( ) ) , m_ProdExpireDate ( prodOrderInfo . get_expireDateTime ( ) ) , m_iTimezoneOffset ( prodOrderInfo . get_timezoneOffset ( ) ) , m_CycleNum ( 1 ) , m_ProrateRules ( prorate_rules ) , m_pCycle ( CleanCycleFactory :: get_cleanCycle ( cycleType , nCycleUnit ) )
	TEST_FIXTURE(TestFixture,TestCProdBillCycleImpl_CProdBillCycleImpl_CProdBillCycleImpl)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float CProdBillCycleImpl :: get_discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestCProdBillCycleImpl_CProdBillCycleImpl_get_discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: getICleanCycle ( ) const
	TEST_FIXTURE(TestFixture,TestCProdBillCycleImpl_getICleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: get_cycleNum ( ) const
	TEST_FIXTURE(TestFixture,TestCProdBillCycleImpl_get_cycleNum)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CProdCycle ( )
	TEST_FIXTURE(TestFixture,TestCProdCycle_CProdCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: CProdCycle :: CProdCycle ( const int64 doneCode , const MXdr :: SRateProdId & prodOrderInfo , const MXdr :: SRateProdBillCycle & prodBillCycle , const int64 llProcessDate , const SProrateRule & prorate_rules ) : m_llProcessDate ( llProcessDate ) , CBillCycle ( getICleanCycle ( ) , prodBillCycle . get_firstCycleDateTime ( ) , prodBillCycle . get_validDateTime ( ) , prodBillCycle . get_expireDateTime ( ) ) , CProdBillCycleImpl ( doneCode , prodOrderInfo , CleanCycleFactory :: toEnumCycleType ( prodBillCycle . get_cycleType ( ) ) , prodBillCycle . get_cycleUnit ( ) , prorate_rules ) , m_iPreCycleCount ( prodBillCycle . get_cycleCount ( ) < 1 ? 1 : prodBillCycle . get_cycleCount ( ) )
	TEST_FIXTURE(TestFixture,TestCProdCycle_CProdCycle_CProdCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: void CProdCycle :: cal_validAndExpireDate ( int64 & llRealValidDate , int64 & llRealExpireDate , int64 & llHopeValidDate , int64 & llHopeExpireDate , const bool IsRefProdExpireDate , const bool IsRefProdValidDate ) const
	TEST_FIXTURE(TestFixture,TestCProdCycle_CProdCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const int32 CProdCycle :: get_cycleNum ( ) const
	TEST_FIXTURE(TestFixture,TestCProdCycle_CProdCycle_get_cycleNum)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 CBillCycleFactory :: cal_prodBillCycle ( const int64 doneCode , const MXdr :: SRateProdId & rateProdInfo , const MXdr :: SRateProdBillCycle & rateProdCycle , const int64 processTime , int64 & llValidDate , int64 & llExpireDate )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_cal_prodBillCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: void CBillCycleFactory :: changeRefModeType ( const int64 llDoneCodeIn , const MXdr :: SRateProdBillCycleMap & rateProdCycleMap , RefModeType & refModeTypeInOut )
	TEST_FIXTURE(TestFixture,TestCBillCycleFactory_changeRefModeType)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
}
