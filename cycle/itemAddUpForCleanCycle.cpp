/* $Id: itemAddUpForCleanCycle.cpp 323781 2013-05-21 03:36:34Z caiwei $  */

#include "itemAddUpForCleanCycle.h"
//#include <base/datetime.h>
//#include <ASSERT.h>

BEGIN_NAMESPACE

namespace{
	//增加 eInvalidCycle类型 戚剑锋	2012-03-19
	const CycleType szEnumCycleTyp[10] = {eInvalidCycle, eDay, eWeek, eMonth, eYear, eDynamicMonth, eHourDynamic, eMonthDynamic, eDayDynamic, eOnce};
}

//增加 eInvalidCycle类型 戚剑锋	2012-03-19
const CycleType CleanCycleFactory::toEnumCycleType(const int32 nCycleType)
{
//	ASSERT(isValidToCycleType(nCycleType));
//	const int32 nI = nCycleType - 1u;
//	
//	return szEnumCycleTyp[nI];
//
	const CycleType ret = (isValidToCycleType(nCycleType) ? szEnumCycleTyp[nCycleType] : eInvalidCycle);
	return ret;
}


/*
	订购周期内的折扣类型. 
*/
class IHalfType
{
public:
	virtual ~IHalfType(){}
public:
	//! @brief 计算订购周期内的折扣. 
	//! @param[in] nX	周期天数/ 秒数. 
	//! @param[in] nY	订购周期内的有效天数/ 秒数. 
	virtual float cal_discount(const int32 nX, const int32 nY) const = 0;
};

/*
	半周期折扣. 
*/
class HalfType: public IHalfType
{
public:
	float cal_discount(const int32 nX, const int32 nY) const
	{
		float fDiscount=1.0f;
		((nX - nY) >= (nX / 2)) ? fDiscount = 0.5f : fDiscount = 1.0f;
		return fDiscount;
	}
};

/*
	无折扣. 
*/
class CycType: public IHalfType
{
public:
	float cal_discount(const int32 nX, const int32 nY) const {return 1.0f;}
};

/*
	按天数/ 秒数折算. 
*/
class DayType: public IHalfType
{
public:
	float cal_discount(const int32 nX, const int32 nY) const
	{
		const float fDiscount = ((float)nY) / nX;
		return fDiscount;
	}
};

/*
	移动规范公式按天折扣
*/
class DayTypeOfCMCC: public IHalfType
{
public:
	float cal_discount(const int32 nX, const int32 nY) const
	{
		float fDiscount = 1.0f;
		if(nX != nY)
		{
			fDiscount = ((float)nY)*12 / 365;
		}
		return fDiscount;
	}
};


//! 析构.
ICleanCycle::~ICleanCycle()
{
	delete m_half;
}

const IHalfType* ICleanCycle::create_halfObject(const int32 nProrateType) const
{
	const IHalfType* _half = NULL;
	
	if (nProrateType == 2){
		_half = new DayType();
	}else if (nProrateType == 1){
		_half = new HalfType();
	}else if (nProrateType == 3){ //移动规范公式按天折扣
		if(m_nCycleType == eMonth || m_nCycleType == eDynamicMonth)
		{
			_half = new DayTypeOfCMCC();
		}
		else
		{
			_half = new DayType();
		}
	}else{
		//! 不折算. 
		_half = new CycType();
	}
	
	ASSERT(_half != NULL);
	return _half;
}

ICleanCycle::ICleanCycle(const CycleType cycleType, const int32 nCycleUnit): m_nCycleType(cycleType), m_nCycleUnit(nCycleUnit), m_half(NULL)
{
}

const float ICleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate, const int32 nProrateType) const throw (const CExpX&)
{
	if (m_half != NULL){delete m_half;}
	m_half = create_halfObject(nProrateType);
	
	return cal_Discount(llRealValidDate, llRealExpireDate, llHopeValidDate, llHopeExpireDate);
}

/*
	按日清零周期. 精确到日. 
*/
class DayCleanCycle: public ICleanCycle
{
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&);
public:	
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	
public:
	DayCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~DayCleanCycle(){}
};

/*
	自然周清零周期. 精确到日. 
*/
class NatureWeekCleanCycle: public ICleanCycle
{
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&);
public:	
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	
public:
	NatureWeekCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~NatureWeekCleanCycle(){}
};

/*
	自然月清零周期. 精确到日. 
*/
class NatureMonthCleanCycle: public ICleanCycle
{
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&);
public:	
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	
public:
	NatureMonthCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~NatureMonthCleanCycle(){}
};

/*
	自然季清零周期. 精确到日. 
*/
//class NatureQuarterCleanCycle: public ICleanCycle
//{
//public:
//	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const;
//public:	
//	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
//	
//public:
//	NatureQuarterCleanCycle(const CycleType cycleType, const int32 nCycleUnit, const int32 nProrateType, const rEffectType effectType): ICleanCycle(cycleType, nCycleUnit, nProrateType, effectType){}
//	~NatureQuarterCleanCycle(){}
//};

/*
	自然年清零周期. 精确到日. 
*/
class NatureYearCleanCycle: public ICleanCycle
{
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&);
public:	
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	
public:
	NatureYearCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~NatureYearCleanCycle(){}
};

/*
	按动态月清零周期. 类似ORACLE提供的add_months()函数的功能. 精确到日. 
*/
class DynamicMonthCleanCycle: public ICleanCycle
{
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&);
public:	
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	
public:
	DynamicMonthCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~DynamicMonthCleanCycle(){}
};

/*
	按小时偏移清零周期. 精确到时分秒. 
*/
class HourDynamicCleanCycle: public ICleanCycle
{
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&);
public:	
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	
public:
	HourDynamicCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~HourDynamicCleanCycle(){}
};

/*
	按自然月偏移清零周期. 精确到时分秒. 
*/
class MonthDynamicCleanCycle: public ICleanCycle
{
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&);
public:	
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	
public:
	MonthDynamicCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~MonthDynamicCleanCycle(){}
};

class OnceCleanCycle: public ICleanCycle
{
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&);
public:	
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	
public:
	OnceCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~OnceCleanCycle(){}
};

/*
	无效时间    2012-03-19添加 戚剑锋 
*/
class InvalidCleanCycle : public ICleanCycle
{
public:
	InvalidCleanCycle(const CycleType cycleType, const int32 nCycleUnit): ICleanCycle(cycleType, nCycleUnit){}
	~InvalidCleanCycle(){}
public:
	int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&)
	{
		throw CExpX(622016);
		//_ERROE_MSG   object: CYCLE_TYPE, error msg: Invalid CYCLE_TYPE. When CYCLE_TYPE is not in range of [1,8], InvalidCleanCycle can't Call cal_validAnExpireData function, suggest: check cycly type
	}
public:
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
	{
		throw CExpX(622017);
		//_ERROE_MSG   object: CYCLE_TYPE, error msg: Invalid CYCLE_TYPE. When CYCLE_TYPE is not in range of [1,8], InvalidCleanCycle can't Call cal_Discount function, suggest: check cycly type
	}
};

//增加 eInvalidCycle类型 戚剑锋	2012-03-19
const ICleanCycle* CleanCycleFactory::get_cleanCycle(const CycleType cycleType, const int32 nCycleUnit)
{
	if (isValidToCycleType(cycleType))
	{
		ASSERT(nCycleUnit >= 1);
	}
	RATING_TRACES("get_cleanCycle::cycleType[%d]", cycleType);
	ICleanCycle* pCleanCycle = NULL;
	switch (cycleType)
	{
	case eDay:
		{
			pCleanCycle = new DayCleanCycle(cycleType, nCycleUnit);
		}
		break;
	case eWeek:
		{
			pCleanCycle = new NatureWeekCleanCycle(cycleType, nCycleUnit);
		}
		break;
	case eMonth:
		{
			pCleanCycle = new NatureMonthCleanCycle(cycleType, nCycleUnit);
		}
		break;
	case eYear:
		{
			pCleanCycle = new NatureYearCleanCycle(cycleType, nCycleUnit);
		}
		break;
	case eDynamicMonth:
		{
			pCleanCycle = new DynamicMonthCleanCycle(cycleType, nCycleUnit);
		}
		break;
	case eHourDynamic:
		{
			pCleanCycle = new HourDynamicCleanCycle(cycleType, nCycleUnit);
		}
		break;
	case eMonthDynamic:
		{
			pCleanCycle = new MonthDynamicCleanCycle(cycleType, nCycleUnit);
		}
		break;
	case eDayDynamic:
		{
			pCleanCycle = new HourDynamicCleanCycle(cycleType, 24u * nCycleUnit);
		}
		break;
	case eOnce:
		{
			pCleanCycle = new OnceCleanCycle(cycleType, nCycleUnit);
		}
		break;
	default:
			pCleanCycle = new InvalidCleanCycle(cycleType, nCycleUnit);
		break;
	}
	
	return pCleanCycle;
}

int32 DayCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const throw (const CExpX&)
{	
	if (llBookDate > llProcessDate)
	{
		throw CExpX(622018)<< llBookDate<< llProcessDate;
		//_ERROE_MSG   object: bookdate ?, error msg: bookdate[?] should be smaller than processDate[?], suggest: check book date and start date in xdr.
	}
	//ASSERT(llBookDate <= llProcessDate);
	
	CBSDateTime processDate = llProcessDate;
	CBSDateTime bookDate = llBookDate;
	processDate = processDate.addSecs(3600*iTimezoneOffset);
	bookDate = bookDate.addSecs(3600*iTimezoneOffset);
//	const CBSDate processDate = toDate(llProcessDate);
//	const CBSDate bookDate = toDate(llBookDate);
	const int32 nB = bookDate.daysTo(processDate) / m_nCycleUnit;
	
	CBSDateTime validDate = bookDate.addDays(nB * m_nCycleUnit).date();
	CBSDateTime expireDate = bookDate.addDays((nB + 1) * m_nCycleUnit).date();
	validDate = validDate.addSecs(0 - 3600*iTimezoneOffset);
	expireDate = expireDate.addSecs(0 - 3600*iTimezoneOffset);

	//ASSERT(validDate <= processDate && expireDate > processDate);
	
	llValidDate = validDate;
	llExpireDate = expireDate;
	
	return nB + 1;
	//}
}

const float DayCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{	
	float fDiscount = 1.0f;
	ASSERT(llRealExpireDate >= llHopeValidDate);
	ASSERT(llRealValidDate <= llHopeExpireDate);
	ASSERT(llHopeValidDate < llHopeExpireDate);
	
	if (llRealValidDate != llHopeValidDate || llHopeExpireDate != llRealExpireDate){
		const int32 _nX = m_nCycleUnit;
		int32 nY = 0;
		
		if (llRealValidDate != llHopeValidDate)
		{
			const CBSDate prodValidDate = llRealValidDate.date();
			const CBSDate validDate = llHopeValidDate.date();
			
			nY += validDate.daysTo(prodValidDate);
		}
		
		if (llHopeExpireDate != llRealExpireDate)
		{
			const CBSDate prodExpireDate = llRealExpireDate.date();
			const CBSDate expireDate = llHopeExpireDate.date();
			
			const int32 nY_Expire = prodExpireDate.daysTo(expireDate);
			nY += nY_Expire;
		}
		
		ASSERT(_nX - nY >= 0);
		fDiscount = m_half->cal_discount(_nX, _nX - nY);
	}
	
	return fDiscount;
}

int32 NatureWeekCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const throw (const CExpX&)
{
	if (llBookDate > llProcessDate)
	{
		throw CExpX(622018)<< llBookDate<< llProcessDate;
		//_ERROE_MSG   object: bookdate ?, error msg: bookdate[?] should be smaller than processDate[?], suggest: check book date and start date in xdr.
    }
	//ASSERT(llBookDate <= llProcessDate);
	
//	const CBSDate processDate = toDate(llProcessDate);
//	const CBSDate bookDate = toDate(llBookDate);
	CBSDateTime processDate = llProcessDate;
	CBSDateTime bookDate = llBookDate;
	processDate = processDate.addSecs(3600*iTimezoneOffset);
	bookDate = bookDate.addSecs(3600*iTimezoneOffset);
	const CBSDate _vB = bookDate.date().addDays(1 - bookDate.dayOfWeek());
	
	const int32 _nX = m_nCycleUnit * 7;
	const int32 nB = _vB.daysTo(processDate.date()) / _nX;
	
	CBSDateTime validDate = _vB.addDays(nB * _nX);
	CBSDateTime expireDate = _vB.addDays((nB + 1) * _nX);
	validDate = validDate.addSecs(0 - 3600*iTimezoneOffset);
	expireDate = expireDate.addSecs(0 - 3600*iTimezoneOffset);
	
	//ASSERT(validDate <= processDate && expireDate > processDate);
	
	llValidDate = validDate;
	llExpireDate = expireDate;
								 
	return nB + 1;
	//}
}

const float NatureWeekCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	float fDiscount = 1.0f;
	ASSERT(llRealExpireDate >= llHopeValidDate);
	ASSERT(llRealValidDate <= llHopeExpireDate);
	ASSERT(llHopeValidDate < llHopeExpireDate);
	
	if (llRealValidDate != llHopeValidDate || llHopeExpireDate != llRealExpireDate){
		const int32 _nX = m_nCycleUnit * 7;
		int32 nY = 0;
		
		if (llRealValidDate != llHopeValidDate)
		{
			const CBSDate prodValidDate = llRealValidDate.date();
			const CBSDate validDate = llHopeValidDate.date();
			
			nY += validDate.daysTo(prodValidDate);
		}
		
		if (llHopeExpireDate != llRealExpireDate )
		{
			const CBSDate prodExpireDate = llRealExpireDate.date();
			const CBSDate expireDate = llHopeExpireDate.date();
			
			const int32 nY_Expire = prodExpireDate.daysTo(expireDate);
			nY += nY_Expire;
		}
		
		ASSERT(_nX - nY >= 0);
		fDiscount = m_half->cal_discount(_nX, _nX - nY);
	}
	
	return fDiscount;
}

int32 NatureMonthCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const throw (const CExpX&)
{
	if (llBookDate > llProcessDate)
	{
		throw CExpX(622018)<< llBookDate<< llProcessDate;
		//_ERROE_MSG   object: bookdate ?, error msg: bookdate[?] should be smaller than processDate[?], suggest: check book date and start date in xdr.
	}
	//ASSERT(llBookDate <= llProcessDate);
	
//	const CBSDate processDate = toDate(llProcessDate);
//	const CBSDate bookDate = toDate(llBookDate);
	CBSDateTime processDate = llProcessDate;
	CBSDateTime bookDate = llBookDate;
	processDate = processDate.addSecs(3600*iTimezoneOffset);
	bookDate = bookDate.addSecs(3600*iTimezoneOffset);
		
	const int32 nB = ((processDate.year() - bookDate.year()) * 12 + processDate.month() - bookDate.month()) / m_nCycleUnit;
	CBSDateTime validDate = bookDate.date().addMonths2(nB * m_nCycleUnit);
	CBSDateTime expireDate = bookDate.date().addMonths2((nB + 1) * m_nCycleUnit);
	validDate = validDate.addDays(1 - validDate.day());		//自然月 转变成1日
	expireDate = expireDate.addDays(1 - expireDate.day());
	
	validDate = validDate.addSecs(0 - 3600*iTimezoneOffset);
	expireDate = expireDate.addSecs(0 - 3600*iTimezoneOffset);
	
	llValidDate = validDate;
	llExpireDate = expireDate;

	return nB + 1;
//	}
}

const float NatureMonthCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	float fDiscount = 1.0f;
	ASSERT(llRealExpireDate >= llHopeValidDate);
	ASSERT(llRealValidDate <= llHopeExpireDate);
	ASSERT(llHopeValidDate < llHopeExpireDate);
	
	if (llRealValidDate != llHopeValidDate || llHopeExpireDate != llRealExpireDate){
		const CBSDate validDate = llHopeValidDate.date();
		const CBSDate expireDate = llHopeExpireDate.date();
		
		const int32 _nX = validDate.daysTo(expireDate);
		int32 nY = 0;
		
		if (llRealValidDate != llHopeValidDate)
		{
			const CBSDate prodValidDate = llRealValidDate.date();
			nY += validDate.daysTo(prodValidDate);
		}
		
		if (llHopeExpireDate != llRealExpireDate)
		{
			const CBSDate prodExpireDate = llRealExpireDate.date();
			const int32 nY_Expire = prodExpireDate.daysTo(expireDate);
			nY += nY_Expire;
		}
		
		ASSERT(_nX - nY >= 0);
		fDiscount = m_half->cal_discount(_nX, _nX - nY);
	}
	
	return fDiscount;
}

//int32 NatureQuarterCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const
//{
//	ASSERT(llBookDate <= llProcessDate);
//	
//	const CBSDate processDate = toDate(llProcessDate);
//	const int32 m = processDate.month();
//	
//	int32 begin = 1;
//	int32 end = 3;
//	for (int32 i = 0; i < 4; ++i){
//		if (m >= begin && m <= end)
//		{
//			llValidDate = (processDate.year() * 10000 + begin * 100 + 1) * 1000000ll;
//			if (end != 12){
//				llExpireDate = (processDate.year() * 10000 + (end + 1) * 100 + 1) * 1000000ll;
//			}else{
//				llExpireDate = ((processDate.year() + 1) * 10000 + 1 * 100 + 1) * 1000000ll;
//			}
//			break;
//		}
//		begin += 3;
//		end += 3;
//	}	
//}

//bool NatureQuarterCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate, float& fDiscount) const
//{
//	bool bDiscountCycle = false;
//	ASSERT(llRealExpireDate >= llHopeValidDate);
//	ASSERT(llRealValidDate <= llHopeExpireDate);
//	ASSERT(llHopeValidDate < llHopeExpireDate);
//	
//	if (llRealValidDate > llHopeValidDate || llHopeExpireDate != llRealExpireDate){
//		const CBSDate validDate = llHopeValidDate.date();
//		const CBSDate expireDate = llHopeExpireDate.date();
//		
//		const int32 _nX = validDate.daysTo(expireDate);
//		int32 nY = 0;
//		
//		if (llRealValidDate > llHopeValidDate)
//		{
//			const CBSDate prodValidDate = llRealValidDate.date();
//			nY += validDate.daysTo(prodValidDate);
//		}
//		
//		if (llHopeExpireDate != llRealExpireDate && m_effectType == eBoth)
//		{
//			const CBSDate prodExpireDate = llRealExpireDate.date();
//			const int32 nY_Expire = prodExpireDate.daysTo(expireDate);
//			nY += nY_Expire;
//		}
//		
//		ASSERT(_nX - nY >= 0);
//		
//		bDiscountCycle = true;
//		fDiscount = m_half->cal_discount(_nX, _nX - nY);
//	}
//	
//	return bDiscountCycle;
//}

int32 NatureYearCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const throw (const CExpX&)
{
	if (llBookDate > llProcessDate)
	{
		throw CExpX(622018)<< llBookDate<< llProcessDate;
		//_ERROE_MSG   object: bookdate ?, error msg: bookdate[?] should be smaller than processDate[?], suggest: check book date and start date in xdr.
	}
	//ASSERT(llBookDate <= llProcessDate);
	
//	const CBSDate processDate = toDate(llProcessDate);
//	const CBSDate bookDate = toDate(llBookDate);
	CBSDateTime processDate = llProcessDate;
	CBSDateTime bookDate = llBookDate;
	processDate = processDate.addSecs(3600*iTimezoneOffset);
	bookDate = bookDate.addSecs(3600*iTimezoneOffset);
		
	const int32 nB = (processDate.year() - bookDate.year()) / m_nCycleUnit;
	const CBSDate validDateTemp = bookDate.date().addMonths2(nB * m_nCycleUnit * 12);
	const CBSDate expireDateTemp = bookDate.date().addMonths2((nB + 1) * m_nCycleUnit * 12);
	
	CBSDateTime validDate(validDateTemp.year(), 1, 1, 0, 0, 0);	//某年1月1日
	CBSDateTime expireDate(expireDateTemp.year(), 1, 1, 0, 0, 0);
	validDate = validDate.addSecs(0 - 3600*iTimezoneOffset);
	expireDate = expireDate.addSecs(0 - 3600*iTimezoneOffset);

	llValidDate = validDate;
	llExpireDate = expireDate;
	
	return nB + 1;
//	}
}

const float NatureYearCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	float fDiscount = 1.0f;
	ASSERT(llRealExpireDate >= llHopeValidDate);
	ASSERT(llRealValidDate <= llHopeExpireDate);
	ASSERT(llHopeValidDate < llHopeExpireDate);
	
	if (llRealValidDate != llHopeValidDate || llHopeExpireDate != llRealExpireDate){
		const CBSDate validDate = llHopeValidDate.date();
		const CBSDate expireDate = llHopeExpireDate.date();
		
		const int32 _nX = validDate.daysTo(expireDate);
		int32 nY = 0;
		
		if (llRealValidDate != llHopeValidDate)
		{
			const CBSDate prodValidDate = llRealValidDate.date();
			nY += validDate.daysTo(prodValidDate);
		}
		
		if (llHopeExpireDate != llRealExpireDate )
		{
			const CBSDate prodExpireDate = llRealExpireDate.date();
			const int32 nY_Expire = prodExpireDate.daysTo(expireDate);
			nY += nY_Expire;
		}
		
		ASSERT(_nX - nY >= 0);
		fDiscount = m_half->cal_discount(_nX, _nX - nY);
	}
	
	return fDiscount;
}

int32 DynamicMonthCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const throw (const CExpX&)
{
	if (llBookDate > llProcessDate)
	{
		throw CExpX(622018)<< llBookDate<< llProcessDate;
		//_ERROE_MSG   object: bookdate ?, error msg: bookdate[?] should be smaller than processDate[?], suggest: check book date and start date in xdr.
	}
	//ASSERT(llBookDate <= llProcessDate);
	
	const CBSDate processDate = llProcessDate.date();
	const CBSDate bookDate = llBookDate.date();
	
	const int32 nY = (processDate.year() - bookDate.year()) * 12 + processDate.month() - bookDate.month();
	int32 nB = nY / m_nCycleUnit;
	
	const int32 _nX = nY - nB * m_nCycleUnit;
	if (_nX == 0 && nB > 0 && processDate.day() < bookDate.day()){--nB;}
	
	const CBSDate validDate = bookDate.addMonths2(nB * m_nCycleUnit);
	const CBSDate expireDate = bookDate.addMonths2((nB + 1) * m_nCycleUnit);
	
	llValidDate = validDate;
	llExpireDate = expireDate;
	
	return nB + 1;
}

const float DynamicMonthCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	float fDiscount = 1.0f;
	ASSERT(llRealExpireDate >= llHopeValidDate);
	ASSERT(llRealValidDate <= llHopeExpireDate);
	ASSERT(llHopeValidDate < llHopeExpireDate);
	if (llRealValidDate != llHopeValidDate || llHopeExpireDate != llRealExpireDate){
		const CBSDate validDate = llHopeValidDate.date();
		const CBSDate expireDate = llHopeExpireDate.date();
		
		const int32 _nX = validDate.daysTo(expireDate);
		int32 nY = 0;
		
		if (llRealValidDate != llHopeValidDate)
		{
			const CBSDate prodValidDate = llRealValidDate.date();
			nY += validDate.daysTo(prodValidDate);
		}
		
		if (llHopeExpireDate != llRealExpireDate )
		{
			const CBSDate prodExpireDate = llRealExpireDate.date();
			const int32 nY_Expire = prodExpireDate.daysTo(expireDate);
			nY += nY_Expire;
		}
		
		ASSERT(_nX - nY >= 0);
		fDiscount = m_half->cal_discount(_nX, _nX - nY);
	}
	
	return fDiscount;
}

int32 HourDynamicCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const throw (const CExpX&)
{
	if (llBookDate > llProcessDate)
	{
		throw CExpX(622018)<< llBookDate<< llProcessDate;
		//_ERROE_MSG   object: bookdate ?, error msg: bookdate[?] should be smaller than processDate[?], suggest: check book date and start date in xdr.
	}
	//ASSERT(llBookDate <= llProcessDate);
	const CBSDateTime processDate = llProcessDate;
	
	const int32 _nX = 3600 * m_nCycleUnit;
	const CBSDateTime bookDate = llBookDate;
	const int32 nB = (bookDate.secsTo(processDate) / _nX);
	
	const CBSDateTime validDate = bookDate.addSecs(nB * _nX);
	const CBSDateTime expireDate = bookDate.addSecs((nB + 1) * _nX);
	
	ASSERT(validDate <= processDate && expireDate > processDate);
	
	llValidDate = validDate;
	llExpireDate = expireDate;
	
	return nB + 1;
}

const float HourDynamicCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	float fDiscount = 1.0f;
	ASSERT(llRealExpireDate >= llHopeValidDate);
	ASSERT(llRealValidDate <= llHopeExpireDate);
	ASSERT(llHopeValidDate < llHopeExpireDate);
	
	if (llRealValidDate != llHopeValidDate || llHopeExpireDate != llRealExpireDate){
		const CBSDateTime validDate = llHopeValidDate;
		const CBSDateTime expireDate = llHopeExpireDate;
		
		const int32 _nX = validDate.secsTo(expireDate);
		int32 nY = 0;
		
		if (llRealValidDate != llHopeValidDate)
		{
			const CBSDateTime prodValidDate = llRealValidDate;
			nY += validDate.secsTo(prodValidDate);
		}
		
		if (llHopeExpireDate != llRealExpireDate )
		{
			const CBSDateTime prodExpireDate = llRealExpireDate;
			const int32 nY_Expire = prodExpireDate.secsTo(expireDate);
			nY += nY_Expire;
		}
		
		ASSERT(_nX - nY >= 0);
		fDiscount = m_half->cal_discount(_nX, _nX - nY);
	}
	
	return fDiscount;
}

int32 MonthDynamicCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const throw (const CExpX&)
{
	if (llBookDate > llProcessDate)
	{
		throw CExpX(622018)<< llBookDate<< llProcessDate;
		//_ERROE_MSG   object: bookdate ?, error msg: bookdate[?] should be smaller than processDate[?], suggest: check book date and start date in xdr.
	}
	//ASSERT(llBookDate <= llProcessDate);
	
	const CBSDateTime processDate = llProcessDate;
	const CBSDateTime bookDate =llBookDate;
	
	const int32 nY = (processDate.year() - bookDate.year()) * 12 + processDate.month() - bookDate.month();
	int32 nB = nY / m_nCycleUnit;
	
	const int32 _nX = nY - nB * m_nCycleUnit;
	if (_nX == 0 && nB > 0){
		const int32 d_p = processDate.day();
		const int32 d_b = bookDate.day();
		if (d_p < d_b){
			--nB;
		}else if (d_p == d_b){
			const int32 h_p = processDate.hour();
			const int32 h_b = bookDate.hour();
			if (h_p < h_b){
				--nB;
			}else if (h_p == h_b){
				const int32 mi_p = processDate.minute();
				const int32 s_p = processDate.second();
				
				const int32 mi_b = bookDate.minute();
				const int32 s_b = bookDate.second();
				
				if (mi_p * 100 + s_p < mi_b * 100 + s_b){--nB;}
			}
		}
	}
	
	const CBSDate validDate = bookDate.date().addMonths2(nB * m_nCycleUnit);
	const CBSDate expireDate = bookDate.date().addMonths2((nB + 1) * m_nCycleUnit);
	
	llValidDate = llBookDate;
	llExpireDate = llBookDate;
	llValidDate.setDate(validDate);
	llExpireDate.setDate(expireDate);
	
	return nB + 1;
}

const float MonthDynamicCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	float fDiscount = 1.0f;
	ASSERT(llRealExpireDate >= llHopeValidDate);
	ASSERT(llRealValidDate <= llHopeExpireDate);
	ASSERT(llHopeValidDate < llHopeExpireDate);
	
	if (llRealValidDate != llHopeValidDate || llHopeExpireDate != llRealExpireDate){
		const CBSDateTime validDate = llHopeValidDate;
		const CBSDateTime expireDate = llHopeExpireDate;
		
		const int32 _nX = validDate.secsTo(expireDate);
		int32 nY = 0;
		
		if (llRealValidDate != llHopeValidDate)
		{
			const CBSDateTime prodValidDate = llRealValidDate;
			nY += validDate.secsTo(prodValidDate);
		}
		
		if (llHopeExpireDate != llRealExpireDate )
		{
			const CBSDateTime prodExpireDate = llRealExpireDate;
			const int32 nY_Expire = prodExpireDate.secsTo(expireDate);
			nY += nY_Expire;
		}
		
		ASSERT(_nX - nY >= 0);
		fDiscount = m_half->cal_discount(_nX, _nX - nY);
	}
	
	return fDiscount;
}

int32 OnceCleanCycle::cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset) const throw (const CExpX&)
{
	RATING_TRACES("OnceCleanCycle::cal_validAndExpireDate");
	llValidDate = llProcessDate;
	llExpireDate = llProcessDate;
	
	return 1;
}

const float OnceCleanCycle::cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	return 1.0f;
}

END_NAMESPACE

