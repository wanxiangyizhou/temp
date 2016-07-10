#include <unittest_cleanCycle_main.h>

SUITE(SUITE_itemAddUpForCleanCycle_cpp)
{
	//Fuction: const ICleanCycle * CleanCycleFactory :: get_cleanCycle ( const CycleType cycleType , const int32 nCycleUnit )
	TEST_FIXTURE(TestFixture,TestCleanCycleFactory_get_cleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: } const CycleType CleanCycleFactory :: toEnumCycleType ( const int32 nCycleType )
	TEST_FIXTURE(TestFixture,TestCleanCycleFactory_toEnumCycleType)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: cal_discount ( const int32 nX , const int32 nY ) const
	TEST_FIXTURE(TestFixture,TestCycType_cal_discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: DayCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestDayCleanCycle_DayCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float DayCleanCycle :: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestDayCleanCycle_DayCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 DayCleanCycle :: cal_validAndExpireDate ( const int64 llBookDate , const int64 llProcessDate , int64 & llValidDate , int64 & llExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestDayCleanCycle_DayCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: cal_discount ( const int32 nX , const int32 nY ) const
	TEST_FIXTURE(TestFixture,TestDayType_cal_discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: cal_discount ( const int32 nX , const int32 nY ) const
	TEST_FIXTURE(TestFixture,TestDayTypeOfCMCC_cal_discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: DynamicMonthCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestDynamicMonthCleanCycle_DynamicMonthCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float DynamicMonthCleanCycle :: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestDynamicMonthCleanCycle_DynamicMonthCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 DynamicMonthCleanCycle :: cal_validAndExpireDate ( const int64 llBookDate , const int64 llProcessDate , int64 & llValidDate , int64 & llExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestDynamicMonthCleanCycle_DynamicMonthCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: cal_discount ( const int32 nX , const int32 nY ) const
	TEST_FIXTURE(TestFixture,TestHalfType_cal_discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: HourDynamicCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestHourDynamicCleanCycle_HourDynamicCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float HourDynamicCleanCycle :: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestHourDynamicCleanCycle_HourDynamicCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 HourDynamicCleanCycle :: cal_validAndExpireDate ( const int64 llBookDate , const int64 llProcessDate , int64 & llValidDate , int64 & llExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestHourDynamicCleanCycle_HourDynamicCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: ICleanCycle :: ~ ICleanCycle ( )
	TEST_FIXTURE(TestFixture,TestICleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: ICleanCycle :: ICleanCycle ( const CycleType cycleType , const int32 nCycleUnit ) : m_nCycleType ( cycleType ) , m_nCycleUnit ( nCycleUnit ) , m_half ( 0 )
	TEST_FIXTURE(TestFixture,TestICleanCycle_ICleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float ICleanCycle :: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate , const int32 nProrateType ) const
	TEST_FIXTURE(TestFixture,TestICleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const IHalfType * ICleanCycle :: create_halfObject ( const int32 nProrateType ) const
	TEST_FIXTURE(TestFixture,TestICleanCycle_create_halfObject)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: IHalfType ( )
	TEST_FIXTURE(TestFixture,TestIHalfType_IHalfType)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: InvalidCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestInvalidCleanCycle_InvalidCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestInvalidCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: cal_validAndExpireDate ( const int64 llBookDate , const int64 llProcessDate , int64 & llValidDate , int64 & llExpireDate , const int32 iTimezoneOffset = 0 ) const
	TEST_FIXTURE(TestFixture,TestInvalidCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: MonthDynamicCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestMonthDynamicCleanCycle_MonthDynamicCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float MonthDynamicCleanCycle :: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestMonthDynamicCleanCycle_MonthDynamicCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 MonthDynamicCleanCycle :: cal_validAndExpireDate ( const int64 llBookDate , const int64 llProcessDate , int64 & llValidDate , int64 & llExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestMonthDynamicCleanCycle_MonthDynamicCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: NatureMonthCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestNatureMonthCleanCycle_NatureMonthCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float NatureMonthCleanCycle :: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestNatureMonthCleanCycle_NatureMonthCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 NatureMonthCleanCycle :: cal_validAndExpireDate ( const int64 llBookDate , const int64 llProcessDate , int64 & llValidDate , int64 & llExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestNatureMonthCleanCycle_NatureMonthCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: NatureWeekCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestNatureWeekCleanCycle_NatureWeekCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float NatureWeekCleanCycle :: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestNatureWeekCleanCycle_NatureWeekCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 NatureWeekCleanCycle :: cal_validAndExpireDate ( const int64 llBookDate , const int64 llProcessDate , int64 & llValidDate , int64 & llExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestNatureWeekCleanCycle_NatureWeekCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: NatureYearCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestNatureYearCleanCycle_NatureYearCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float NatureYearCleanCycle :: cal_Discount ( const int64 llRealValidDate , const int64 llRealExpireDate , const int64 llHopeValidDate , const int64 llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestNatureYearCleanCycle_NatureYearCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 NatureYearCleanCycle :: cal_validAndExpireDate ( const int64 llBookDate , const int64 llProcessDate , int64 & llValidDate , int64 & llExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestNatureYearCleanCycle_NatureYearCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: OnceCleanCycle ( )
	TEST_FIXTURE(TestFixture,TestOnceCleanCycle_OnceCleanCycle)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: const float OnceCleanCycle :: cal_Discount ( const CBSDateTime & llRealValidDate , const CBSDateTime & llRealExpireDate , const CBSDateTime & llHopeValidDate , const CBSDateTime & llHopeExpireDate ) const
	TEST_FIXTURE(TestFixture,TestOnceCleanCycle_OnceCleanCycle_cal_Discount)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
	//Fuction: int32 OnceCleanCycle :: cal_validAndExpireDate ( const CBSDateTime & llBookDate , const CBSDateTime & llProcessDate , CBSDateTime & llValidDate , CBSDateTime & llExpireDate , const int32 iTimezoneOffset ) const
	TEST_FIXTURE(TestFixture,TestOnceCleanCycle_OnceCleanCycle_cal_validAndExpireDate)
	{
		/*codes for test start here*/


		/*codes for test stop here*/
	}
}
