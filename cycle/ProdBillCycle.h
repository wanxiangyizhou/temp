/* $Id: ProdBillCycle.h 320094 2013-05-10 04:26:41Z caiwei $  */
#ifndef _BILL_CYCLE_H_
#define _BILL_CYCLE_H_

#include <xdr_def_sdl_c.h>

#include "itemAddUpForCleanCycle.h"

BEGIN_NAMESPACE

class SProrateRule;
/*
以下实现约束: 
1.激活时, 先订购的(用户级)产品, 其相关的生效日期包括产品计费周期, 必须调整成激活时间, 另外, 还包括帐期的生效日期.
2.因订购可能早于激活, 所以, 客户级/账户级订购, 集团订购将除外.
例外.
(1)只适用于基于订购生效日期的局数据配置, 或者产品计费周期.
(2)帐期, 仅当资源分摊时(作用到每个成员上), 或者各个成员的帐期一致时. 
*/

/*
约束: 以产品订购为主线的业务, 当需要计算周期时. 
特点: 1.考虑时区. 
*/
class CProdBillCycle
{
public:
	virtual ~CProdBillCycle(){}
public:
	//! 计算(周期性)生失效日期. (可选)参考产品生失效日期.
	virtual void cal_validAndExpireDate(
		CBSDateTime& llRealValidDate,			//返回周期开始时间
		CBSDateTime& llRealExpireDate,		//返回周期结束时间
		CBSDateTime& llHopeValidDate,			//理想情况下的周期开始时间
		CBSDateTime& llHopeExpireDate, 		//理想情况下的周期结束时间
		const bool IsRefProdExpireDate,	//是否要参考产品失效日期
		const bool IsRefProdValidDate	// 是否参考产品生效日期
	) const throw (const CExpX&) = 0;
public:
	//! 计算生失效日期, 参考产品生失效日期.
	inline void cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&)
	{
		CBSDateTime llHopeValidDate, llHopeExpireDate;
		cal_validAndExpireDate(llRealValidDate, llRealExpireDate, llHopeValidDate, llHopeExpireDate, IsRefProdExpireDate, IsRefProdValidDate);
	}
	inline void cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate) const throw (const CExpX&)
	{
		cal_validAndExpireDate(llRealValidDate, llRealExpireDate, true, true);
	}
	inline void cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
	{
		cal_validAndExpireDate(llRealValidDate, llRealExpireDate, llHopeValidDate, llHopeExpireDate, true, true);
	}
public:
	//! 约束. 
	//! 前提条件是: 必须调用cal_validAndExpireDate接口, 否则结果不明确.
	virtual const float get_discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&) = 0;
	
public:
	//! 约束. 
	//! 前提条件是: 必须调用cal_validAndExpireDate接口, 否则结果不明确.
	virtual const int32 get_cycleNum() const = 0;
	
private:
	CProdBillCycle(const CProdBillCycle&);
	CProdBillCycle& operator= (const CProdBillCycle&);
protected:
	CProdBillCycle(){}
};


enum RefModeType{eAccoutCycle = 0u, eProdCycle, eXCBasedOnProd, eXCBasedOnActivation, eXCBasedOnUserAccess, eXCBasedOnActivationEx,eMainProdCycle, eInvalidRefMode};
class CBillCycleFactory
{
public:
	//! 根据RefMode, 获取计费周期的计算接口. 
/*
0、基于产品订购时间, 参考账户账期. 
1、基于产品订购时间, 参考产品计费周期. 
2、基于产品订购时间, 参考局数据配置定义.
3、基于(用户)激活时间, 参考局数据配置定义, 但首周期折扣值是按照订购时间计算. 
4、基于(用户)入网时间, 参考局数据配置定义, 但首周期折扣值是按照订购时间计算. 
5、基于(用户)激活时间, 参考局数据配置定义, 但首周期折扣值是按照激活时间计算. 
*/	
	//! 约束: 不以局数据配置为计费周期参考方式时. 
	//! 违反约束时将打错单. 
	//! 同时, 也不存在首周期折扣的问题. 
	static const CProdBillCycle* get_billCycleWithoutConfig(const int64 doneCode, const CBSDateTime& processTime ,const MXdr::SSubCommon& subCommon, const int32 cycleRefMode) throw (const CExpX&);
	
	//! 注意: 不存在首周期折扣的问题. 
	static const CProdBillCycle* get_billCycleWithConfig(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int32 cycleRefMode, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&);
	
	//! 
	static const CProdBillCycle* get_billCycleWithDiscountAndConfig(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int32 cycleRefMode, const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules) throw (const CExpX&);
public:
	//! tool.
	static const RefModeType toEnumRefModeType(const int32 cycleRefMode);
	//! @param[in] cycleRefMode.
	//! @brief 判断类型是否合法. 
	static bool isValidRefMode(const int32 cycleRefMode){return (cycleRefMode <= 6u && cycleRefMode >= 0u);}
private:
	//! 
	static const CProdBillCycle* get_billCycleWithDiscountAndConfig(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const RefModeType refModeType, const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules) throw (const CExpX&);
	
public://! tool.
	//! 获取当前帐期. 
	static int32 cal_acctBillCycle(const MXdr::SAcctBillCycle& acctBillCycle, const CBSDateTime& processTime, CBSDateTime& llValidDate, CBSDateTime& llExpireDate) throw (const CExpX&);
	static int32 cal_prodBillCycle(const int64 doneCode,const MXdr::SRateProdId& rateProdInfo, const MXdr::SRateProdBillCycle& rateProdCycle, const CBSDateTime& processTime, CBSDateTime& llValidDate, CBSDateTime& llExpireDate) throw (const CExpX&);
public:
	//! @brief 获取失效日期, 用于一次性的场景. 
/*
0、	参考账户账期. 
1、	参考产品计费周期. 
2-5	参考局数据配置定义.
*/ 
	//! 立即生效.
	static CBSDateTime cal_expireDate(const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int64 doneCode, const int32 cycleRefMode, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&);
private:
	static CBSDateTime cal_expireDate(const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int64 doneCode, const RefModeType refModeType, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&);
public:
	//! 检查产品资料的有效性.
	static void checkProd(const int64 llDoneCodeIn, const CBSDateTime& processTime, const RefModeType refModeType,const MXdr::SRateProdIdMap& rateProdMap,int64 &llDoneCodeOut,MXdr::SRateProdId& rateProdInfo) throw (const CExpX&);
public:
	//! 对参考产品周期，同时产品周期参考帐期时，修改枚举值
	static void changeRefModeType(const int64 llDoneCodeIn,const MXdr::SRateProdBillCycleMap& rateProdCycleMap,RefModeType& refModeTypeOut) throw (const CExpX&);
public:
	//! 检查激活资料的有效性.
	static CBSDateTime checkActivation(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&);
public:
	//! 检查入网资料的有效性.
	static CBSDateTime checkUserAccess(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&);
public:
	//! 检查帐期资料的有效性.
	static const MXdr::SAcctBillCycle& checkAcctBillCycle(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&);
	static void checkAcctBillCycle(const MXdr::SAcctBillCycle& acctBillCycle, const CBSDateTime& processTime) throw (const CExpX&);
public:
	//! 检查产品计费周期资料的有效性.
	static const MXdr::SRateProdBillCycle& checkProdBillCycle(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&);
	static void checkProdBillCycle(const int64 doneCode,const MXdr::SRateProdId& rateProdInfo, const MXdr::SRateProdBillCycle& rateProdCycle, const CBSDateTime& processTime) throw (const CExpX&);
};

END_NAMESPACE

#endif
