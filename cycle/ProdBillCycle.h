/* $Id: ProdBillCycle.h 320094 2013-05-10 04:26:41Z caiwei $  */
#ifndef _BILL_CYCLE_H_
#define _BILL_CYCLE_H_

#include <xdr_def_sdl_c.h>

#include "itemAddUpForCleanCycle.h"

BEGIN_NAMESPACE

class SProrateRule;
/*
����ʵ��Լ��: 
1.����ʱ, �ȶ�����(�û���)��Ʒ, ����ص���Ч���ڰ�����Ʒ�Ʒ�����, ��������ɼ���ʱ��, ����, ���������ڵ���Ч����.
2.�򶩹��������ڼ���, ����, �ͻ���/�˻�������, ���Ŷ���������.
����.
(1)ֻ�����ڻ��ڶ�����Ч���ڵľ���������, ���߲�Ʒ�Ʒ�����.
(2)����, ������Դ��̯ʱ(���õ�ÿ����Ա��), ���߸�����Ա������һ��ʱ. 
*/

/*
Լ��: �Բ�Ʒ����Ϊ���ߵ�ҵ��, ����Ҫ��������ʱ. 
�ص�: 1.����ʱ��. 
*/
class CProdBillCycle
{
public:
	virtual ~CProdBillCycle(){}
public:
	//! ����(������)��ʧЧ����. (��ѡ)�ο���Ʒ��ʧЧ����.
	virtual void cal_validAndExpireDate(
		CBSDateTime& llRealValidDate,			//�������ڿ�ʼʱ��
		CBSDateTime& llRealExpireDate,		//�������ڽ���ʱ��
		CBSDateTime& llHopeValidDate,			//��������µ����ڿ�ʼʱ��
		CBSDateTime& llHopeExpireDate, 		//��������µ����ڽ���ʱ��
		const bool IsRefProdExpireDate,	//�Ƿ�Ҫ�ο���ƷʧЧ����
		const bool IsRefProdValidDate	// �Ƿ�ο���Ʒ��Ч����
	) const throw (const CExpX&) = 0;
public:
	//! ������ʧЧ����, �ο���Ʒ��ʧЧ����.
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
	//! Լ��. 
	//! ǰ��������: �������cal_validAndExpireDate�ӿ�, ����������ȷ.
	virtual const float get_discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&) = 0;
	
public:
	//! Լ��. 
	//! ǰ��������: �������cal_validAndExpireDate�ӿ�, ����������ȷ.
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
	//! ����RefMode, ��ȡ�Ʒ����ڵļ���ӿ�. 
/*
0�����ڲ�Ʒ����ʱ��, �ο��˻�����. 
1�����ڲ�Ʒ����ʱ��, �ο���Ʒ�Ʒ�����. 
2�����ڲ�Ʒ����ʱ��, �ο����������ö���.
3������(�û�)����ʱ��, �ο����������ö���, ���������ۿ�ֵ�ǰ��ն���ʱ�����. 
4������(�û�)����ʱ��, �ο����������ö���, ���������ۿ�ֵ�ǰ��ն���ʱ�����. 
5������(�û�)����ʱ��, �ο����������ö���, ���������ۿ�ֵ�ǰ��ռ���ʱ�����. 
*/	
	//! Լ��: ���Ծ���������Ϊ�Ʒ����ڲο���ʽʱ. 
	//! Υ��Լ��ʱ�����. 
	//! ͬʱ, Ҳ�������������ۿ۵�����. 
	static const CProdBillCycle* get_billCycleWithoutConfig(const int64 doneCode, const CBSDateTime& processTime ,const MXdr::SSubCommon& subCommon, const int32 cycleRefMode) throw (const CExpX&);
	
	//! ע��: �������������ۿ۵�����. 
	static const CProdBillCycle* get_billCycleWithConfig(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int32 cycleRefMode, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&);
	
	//! 
	static const CProdBillCycle* get_billCycleWithDiscountAndConfig(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int32 cycleRefMode, const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules) throw (const CExpX&);
public:
	//! tool.
	static const RefModeType toEnumRefModeType(const int32 cycleRefMode);
	//! @param[in] cycleRefMode.
	//! @brief �ж������Ƿ�Ϸ�. 
	static bool isValidRefMode(const int32 cycleRefMode){return (cycleRefMode <= 6u && cycleRefMode >= 0u);}
private:
	//! 
	static const CProdBillCycle* get_billCycleWithDiscountAndConfig(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const RefModeType refModeType, const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules) throw (const CExpX&);
	
public://! tool.
	//! ��ȡ��ǰ����. 
	static int32 cal_acctBillCycle(const MXdr::SAcctBillCycle& acctBillCycle, const CBSDateTime& processTime, CBSDateTime& llValidDate, CBSDateTime& llExpireDate) throw (const CExpX&);
	static int32 cal_prodBillCycle(const int64 doneCode,const MXdr::SRateProdId& rateProdInfo, const MXdr::SRateProdBillCycle& rateProdCycle, const CBSDateTime& processTime, CBSDateTime& llValidDate, CBSDateTime& llExpireDate) throw (const CExpX&);
public:
	//! @brief ��ȡʧЧ����, ����һ���Եĳ���. 
/*
0��	�ο��˻�����. 
1��	�ο���Ʒ�Ʒ�����. 
2-5	�ο����������ö���.
*/ 
	//! ������Ч.
	static CBSDateTime cal_expireDate(const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int64 doneCode, const int32 cycleRefMode, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&);
private:
	static CBSDateTime cal_expireDate(const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int64 doneCode, const RefModeType refModeType, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&);
public:
	//! ����Ʒ���ϵ���Ч��.
	static void checkProd(const int64 llDoneCodeIn, const CBSDateTime& processTime, const RefModeType refModeType,const MXdr::SRateProdIdMap& rateProdMap,int64 &llDoneCodeOut,MXdr::SRateProdId& rateProdInfo) throw (const CExpX&);
public:
	//! �Բο���Ʒ���ڣ�ͬʱ��Ʒ���ڲο�����ʱ���޸�ö��ֵ
	static void changeRefModeType(const int64 llDoneCodeIn,const MXdr::SRateProdBillCycleMap& rateProdCycleMap,RefModeType& refModeTypeOut) throw (const CExpX&);
public:
	//! ��鼤�����ϵ���Ч��.
	static CBSDateTime checkActivation(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&);
public:
	//! ����������ϵ���Ч��.
	static CBSDateTime checkUserAccess(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&);
public:
	//! ����������ϵ���Ч��.
	static const MXdr::SAcctBillCycle& checkAcctBillCycle(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&);
	static void checkAcctBillCycle(const MXdr::SAcctBillCycle& acctBillCycle, const CBSDateTime& processTime) throw (const CExpX&);
public:
	//! ����Ʒ�Ʒ��������ϵ���Ч��.
	static const MXdr::SRateProdBillCycle& checkProdBillCycle(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&);
	static void checkProdBillCycle(const int64 doneCode,const MXdr::SRateProdId& rateProdInfo, const MXdr::SRateProdBillCycle& rateProdCycle, const CBSDateTime& processTime) throw (const CExpX&);
};

END_NAMESPACE

#endif
