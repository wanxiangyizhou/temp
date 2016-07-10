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

//���� eInvalidCycle���� �ݽ���  2012-03-19
enum CycleType{eInvalidCycle = 0u, eDay = 1u, eWeek = 2u, eMonth = 3u, eYear = 4u, eDynamicMonth = 5u, eHourDynamic = 6u, eMonthDynamic = 7u, eDayDynamic = 8u, eOnce = 9u};


class IHalfType;
/*
	�������ڽӿ�. ���������������Դ����Ŀ�ۻ������ɵ�. 
*/
class ICleanCycle
{
public:
	virtual ~ICleanCycle();
protected:
	ICleanCycle(const CycleType cycleType, const int32 nCycleUnit /*,const int32 nProrateType, const rEffectType effectType*/);//yuzj3 2012 08 09 effectType���ô���CycleType
public:
	//! @brief ���ݲ�Ʒ�������ں͵�ǰ��������, �����Ŀ�ۼ���ʧЧ����. ע��: ǰ�����[llHopeValidDate, llHopeExpireDate). 
	//! @param[in] llBookDate	��Ʒ��������(����ʱ����). 
	//! @param[in] llProcessDate	��ǰ��������(����ʱ����). 
	
	//! @exception ��ICleanCycle����Ϊ eInvalidCycleType ʱ, ������ô˺�������ʱ�� ���׳��쳣.
	virtual int32 cal_validAndExpireDate(const CBSDateTime& llBookDate, const CBSDateTime& llProcessDate, CBSDateTime& llValidDate, CBSDateTime& llExpireDate, const int32 iTimezoneOffset = 0) const throw (const CExpX&) = 0;
public:
	//! @brief ���㶩�������ڵ��ۿ���. 
	//! @param[out] fDiscount, �ۿ���. (����>1.0lf)
	//! @return ���������ڷ���true, ���򷵻�false. ֻ�е����ۿ�������ʱ, fDiscount�Żᱻ�޸�.
	const float cal_Discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate, const int32 nProrateType) const throw (const CExpX&);
public:
	//! @brief ָ�����ۿ۲���, �Ƿ���Ĵ����ۿ�.
	bool have_Discount(const int32 nProrateType) const {return (nProrateType >= 1u && nProrateType <= 3u);}
public:
	int32 get_CyleType()const { return static_cast<int32>(m_nCycleType);} 
private:
	//! @exception ��ICleanCycle����Ϊ eInvalidCycleType ʱ, ������ô˺�������ʱ�� �����쳣.
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
	//! @param[in] llValidDate, ��Ʒ��������Ч����. 
	inline const CBSDateTime& cal_real_validDate(const CBSDateTime& llValidDate, const CBSDateTime& llHopeValidDate) const
	{
		return (llHopeValidDate >= llValidDate ? llHopeValidDate : llValidDate);
	}
	//! @param[in] llExpireDate, ��Ʒ������ʧЧ����. 
	inline const CBSDateTime& cal_real_expireDate(const CBSDateTime& llExpireDate, const CBSDateTime& llHopeExpireDate) const
	{
		return (llHopeExpireDate < llExpireDate ? llHopeExpireDate : llExpireDate);
	}
};

/*
	�������ڹ�����. 
*/

class CleanCycleFactory
{
public:
	//! @param[in] ��������cycleType. 
	//! @param[in] ���ڵ�λ/���	nCycleUnit.	����ֵ = cycleType * nCycleUnit.
	//! @brief ���ۿ�. 
	static const ICleanCycle* get_cleanCycle(const CycleType cycleType, const int32 nCycleUnit);
	//������ۿۣ������۰���򣬵���������ʧЧʱ���Ѿ����ɺ�
	const ICleanCycle* get_cleanCycleWithProrateType(const int32 nProrateType);
public:
	//! tool.
	static const CycleType toEnumCycleType(const int32 nCycleType);
	//! @param[in] nCycleType
	//! @brief �ж������Ƿ�Ϸ�. 
	static bool isValidToCycleType(const int32 nCycleType){return (nCycleType <= 9u && nCycleType >= 1u);}
};
END_NAMESPACE

#endif	//_ITEM_ADDUP_FOR_CLEANCYCLE_H_

