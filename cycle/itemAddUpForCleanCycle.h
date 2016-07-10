/* $Id: itemAddUpForCleanCycle.h 323781 2013-05-21 03:36:34Z caiwei $  */
/******************************************************************************
 *	file:	itemAddUpForCleanCycle.h
 *	brief:	
 *			this file implement the interface ICleanCycle and it's implement.
 *
 *	Copyright (c) 2008, 2010 Asiainfo Technologies(China),Inc.
 *
 *	History:
 *			2009-02-12	caiwei	create
 *****************************************************************************/

#ifndef _ITEM_ADDUP_FOR_CLEANCYCLE_H_
#define _ITEM_ADDUP_FOR_CLEANCYCLE_H_

#include <charging_base_def.h>
#include <charging_exp_x.h>
#ifdef AIXV3
#ifdef BIT64
	#include <sys/inttypes.h>
#endif
#endif

BEGIN_NAMESPACE

//增加 eInvalidCycle类型 戚剑锋  2012-03-19
enum CycleType{eInvalidCycle = 0u, eDay = 1u, eWeek = 2u, eMonth = 3u, eYear = 4u, eDynamicMonth = 5u, eHourDynamic = 6u, eMonthDynamic = 7u, eDayDynamic = 8u, eOnce = 9u};


class IHalfType;
/*
	清零周期接口. 用于周期性免费资源、科目累积量生成等. 
*/
class ICleanCycle
{
public:
	virtual ~ICleanCycle();
protected:
	ICleanCycle(const CycleType cycleType, const int32 nCycleUnit /*,const int32 nProrateType, const rEffectType effectType*/);//yuzj3 2012 08 09 effectType不用传入CycleType
public:
	//! @brief 根据产品订购日期和当前处理日期, 计算科目累计生失效日期. 注意: 前开后闭[llHopeValidDate, llHopeExpireDate). 
	//! @param[in] llBookDate	产品订购日期(包括时分秒). 
	//! @param[in] llProcessDate	当前处理日期(包括时分秒). 
	
	//! @exception 当ICleanCycle类型为 eInvalidCycleType 时, 如果调用此函数计算时间 会抛出异常.
	virtual int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&) = 0;
public:
	//! @brief 计算订购周期内的折扣率. 
	//! @param[out] fDiscount, 折扣率. (可能>1.0lf)
	//! @return 折算周期内返回true, 否则返回false. 只有当在折扣周期内时, fDiscount才会被修改.
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate, const int32 nProrateType) const throw (const CExpX&);
public:
	//! @brief 指定的折扣策略, 是否真的存在折扣.
	bool have_Discount(const int32 nProrateType) const {return (nProrateType >= 1u && nProrateType <= 3u);}
public:
	int32 get_CyleType()const { return static_cast<int32>(m_nCycleType);} 
private:
	//! @exception 当ICleanCycle类型为 eInvalidCycleType 时, 如果调用此函数计算时间 会抛异常.
	virtual const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&) = 0;
protected:
	mutable const IHalfType* m_half;
protected:
	const CycleType m_nCycleType;
	const int32 m_nCycleUnit;
private:
	const IHalfType* create_halfObject(const int32 nProrateType) const;
private:
	ICleanCycle& operator=(const ICleanCycle&);
	ICleanCycle(const ICleanCycle&);
	ICleanCycle();
public:
	//! tool.
	//! @param[in] llValidDate, 产品或规则的生效日期. 
	inline const CBSDateTime& cal_real_validDate(const CBSDateTime& llValidDate, const CBSDateTime& llHopeValidDate) const
	{
		return (llHopeValidDate >= llValidDate ? llHopeValidDate : llValidDate);
	}
	//! @param[in] llExpireDate, 产品或规则的失效日期. 
	inline const CBSDateTime& cal_real_expireDate(const CBSDateTime& llExpireDate, const CBSDateTime& llHopeExpireDate) const
	{
		return (llHopeExpireDate < llExpireDate ? llHopeExpireDate : llExpireDate);
	}
};

/*
	清零周期工厂类. 
*/

class CleanCycleFactory
{
public:
	//! @param[in] 周期类型cycleType. 
	//! @param[in] 周期单位/间隔	nCycleUnit.	周期值 = cycleType * nCycleUnit.
	//! @brief 不折扣. 
	static const ICleanCycle* get_cleanCycle(const CycleType cycleType, const int32 nCycleUnit);
	//需计算折扣，传入折半规则，但必须在生失效时间已经生成后
	const ICleanCycle* get_cleanCycleWithProrateType(const int32 nProrateType);
public:
	//! tool.
	static const CycleType toEnumCycleType(const int32 nCycleType);
	//! @param[in] nCycleType
	//! @brief 判断类型是否合法. 
	static bool isValidToCycleType(const int32 nCycleType){return (nCycleType <= 9u && nCycleType >= 1u);}
};
END_NAMESPACE

#endif	//_ITEM_ADDUP_FOR_CLEANCYCLE_H_

