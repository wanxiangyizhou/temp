/* $Id: ProdBillCycle.cpp 320094 2013-05-10 04:26:41Z caiwei $  */
#include "ProdBillCycle.h"
#include "IProrateDeductRule.h"

BEGIN_NAMESPACE

//#define CYCLE_BY_PROD_CYCLE 1
//#define CYCLE_BY_XC_CYCLE 2
//#define CYCLE_BY_USER_CYCLE 0
//#define CYCLE_BY_XC_BASED_ON_ACTIVATION 3
//#define CYCLE_BY_XC_BASED_ON_USER_ACCESS 4

namespace{
	const RefModeType szEnumRefModeTyp[7] = {eAccoutCycle, eProdCycle, eXCBasedOnProd, eXCBasedOnActivation, eXCBasedOnUserAccess, eXCBasedOnActivationEx,eMainProdCycle};
}

const RefModeType CBillCycleFactory::toEnumRefModeType(const int32 cycleRefMode)
{
	const RefModeType ret = (isValidRefMode(cycleRefMode) ? szEnumRefModeTyp[cycleRefMode] : eInvalidRefMode);
	return ret;
}



//! �Ʒ�����ʱ��. �����ǲ�Ʒ�Ʒ�����, Ҳ����������.
class CBillCycle
{
public:
	CBillCycle(const ICleanCycle* pCycle, const CBSDateTime& firstExpireDate, const CBSDateTime& cycleValidDate, const CBSDateTime& cycleExpireDate);
	~CBillCycle()
	{
	}
public:
	const int32 cal_validAndExpireDate(
		const CBSDateTime& llProcessDate,
		CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate,
		const int32 iTimezoneOffset) const throw (const CExpX&);
private:
	const ICleanCycle* const m_pCycle;
protected:
	const CBSDateTime m_cycleValidDate; //���ڹ�����Чʱ��
	const CBSDateTime m_cycleExpireDate; //���ڹ���ʧЧʱ��
	const CBSDateTime m_firstExpireDate;	//�����ڽ�ֹʱ��

	const bool m_bIsHaveFirstExpireDate;//�Ƿ���ڴ�С����
private:
	CBillCycle(const CBillCycle&);
	CBillCycle& operator= (const CBillCycle&);
	CBillCycle();
};

CBillCycle::CBillCycle(const ICleanCycle* pCycle, const CBSDateTime& firstExpireDate, const CBSDateTime& cycleValidDate, const CBSDateTime& cycleExpireDate): m_cycleValidDate(cycleValidDate), m_cycleExpireDate(cycleExpireDate), m_firstExpireDate(firstExpireDate), m_bIsHaveFirstExpireDate(firstExpireDate >= cycleValidDate && firstExpireDate < cycleExpireDate), m_pCycle(pCycle)
{
}

const int32 CBillCycle::cal_validAndExpireDate(
	const CBSDateTime& llProcessDate,
	CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate,
	const int32 iTimezoneOffset)const throw (const CExpX&)
{
	int32 cycleNum;
	if (!m_bIsHaveFirstExpireDate || llProcessDate >= m_firstExpireDate)
	{//! ��������ʹ�����¼����߼�.
		const CBSDateTime llOrderValidDate = m_bIsHaveFirstExpireDate ? m_firstExpireDate : m_cycleValidDate;

		//������ʧЧʱ��
		cycleNum = m_pCycle->cal_validAndExpireDate(llOrderValidDate, llProcessDate, llHopeValidDate, llHopeExpireDate, iTimezoneOffset);
		if (m_bIsHaveFirstExpireDate){++cycleNum;}	//!< ���ڴ�С�����Ҳ��ǵ�һ�����ڣ���������1.

		llRealValidDate = llHopeValidDate > llOrderValidDate ? llHopeValidDate : llOrderValidDate;
		llRealExpireDate = llHopeExpireDate < m_cycleExpireDate ? llHopeExpireDate : m_cycleExpireDate;
		RATING_TRACES("old_valid_date[%s], old_expire_date[%s], valid_date[%s], expire_date[%s]",
		llHopeValidDate.toString("%Y%M%D%H%N%S").c_str(),
		llHopeExpireDate.toString("%Y%M%D%H%N%S").c_str(),
		llRealValidDate.toString("%Y%M%D%H%N%S").c_str(),
		llRealExpireDate.toString("%Y%M%D%H%N%S").c_str());
	}
	else{//! ������.
		m_pCycle->cal_validAndExpireDate(m_cycleValidDate, llProcessDate, llHopeValidDate, llHopeExpireDate, iTimezoneOffset);

		llRealValidDate = m_cycleValidDate;
		llRealExpireDate = m_firstExpireDate;

		cycleNum = 1;
		RATING_TRACES("old_valid_date[%s], old_expire_date[%s], valid_date[%s], expire_date[%s]",
		llHopeValidDate.toString("%Y%M%D%H%N%S").c_str(),
		llHopeExpireDate.toString("%Y%M%D%H%N%S").c_str(),
		llRealValidDate.toString("%Y%M%D%H%N%S").c_str(),
		llRealExpireDate.toString("%Y%M%D%H%N%S").c_str());
	}

	return cycleNum;
}

class CProdBillCycleImpl: public CProdBillCycle
{
public:
	virtual ~CProdBillCycleImpl();
public:
	//! Լ��.
	//! ǰ��������: �������cal_validAndExpireDate�ӿ�, ����������ȷ.
	virtual const float get_discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
public:
	//! Լ��.
	//! ǰ��������: �������cal_validAndExpireDate�ӿ�, ����������ȷ.
	virtual const int32 get_cycleNum() const {return m_CycleNum;}
protected:
	const ICleanCycle* const m_pCycle;
protected:
	const CBSDateTime m_ProdValidDate;	//��Ʒ��Чʱ��.
	const CBSDateTime m_ProdExpireDate;	//��ƷʧЧʱ��.
	const SProrateRule m_ProrateRules;//��ʧЧʱ���۰����
protected:
	mutable int32 m_CycleNum;		//�ڼ�����������. (��ʱ����)
protected:
	const int32 m_iTimezoneOffset;	//ʱ��ƫ��ֵ.
protected:
	const int64 m_llDoneCode;//��Ʒʵ��ID.
protected:
	const ICleanCycle* getICleanCycle() const {return m_pCycle;}
protected:
	CProdBillCycleImpl(const int64 doneCode, const MXdr::SRateProdId& prodOrderInfo, const CycleType cycleType, const int32 nCycleUnit, const SProrateRule& prorate_rules);

private:
	CProdBillCycleImpl(const CProdBillCycleImpl&);
	CProdBillCycleImpl& operator= (const CProdBillCycleImpl&);
	CProdBillCycleImpl();
};

CProdBillCycleImpl::~CProdBillCycleImpl()
{
	delete m_pCycle;
}

CProdBillCycleImpl::CProdBillCycleImpl(const int64 doneCode, const MXdr::SRateProdId& prodOrderInfo, const CycleType cycleType, const int32 nCycleUnit, const SProrateRule& prorate_rules)
: m_llDoneCode(doneCode), m_ProdValidDate(prodOrderInfo.get_validDateTime()), m_ProdExpireDate(prodOrderInfo.get_expireDateTime()), m_iTimezoneOffset(prodOrderInfo.get_timezoneOffset()), m_CycleNum(1), m_ProrateRules(prorate_rules), m_pCycle(CleanCycleFactory::get_cleanCycle(cycleType, nCycleUnit))
{
}

const float CProdBillCycleImpl::get_discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	bool bNeedCal = false;
	int32 prorateType = 0;

	if (llRealExpireDate >= m_ProdExpireDate && m_ProrateRules.prorate_type_last != 0)	//!< ���ڲ�ƷʧЧʱ��˵����β����.
	{
		prorateType = m_ProrateRules.prorate_type_last;
		bNeedCal = true;
	}

	if (1 == get_cycleNum() && m_ProrateRules.prorate_type_first != 0)	//!< ������.  ���ͬʱ��β����, ���������ڹ���Ϊ׼.
	{
		prorateType = m_ProrateRules.prorate_type_first;
		bNeedCal = true;
	}

	return ((!bNeedCal || !m_pCycle->have_Discount(prorateType)) ? 1.0f : m_pCycle->cal_Discount(llRealValidDate, llRealExpireDate, llHopeValidDate, llHopeExpireDate, prorateType));
}

// �״μ���ʱ���ã�llRealValidDateҪ�����״μ������startTime
// ���Һ����ڲ����ж��Ƿ��������ڣ�ֻ�ж��Ƿ���Ҫ�������ۿۣ�һ�������
//const float CProdBillCycleImpl::get_discountFirstActive(
//  const int64 llRealValidDate,
//  const int64 llRealExpireDate,
//  const int64 llHopeValidDate,
//  const int64 llHopeExpireDate) const
//{
//	if (m_ProrateRules.prorate_type_first == 0) //!< ������Ϊ0, ������Ҫ�ۿ�.
//	{
//		return 1.0f;
//	}
//	return m_pCycle->cal_Discount(llRealValidDate, llRealExpireDate, llHopeValidDate, llHopeExpireDate, m_ProrateRules.prorate_type_first);
//}


/*
	��Ʒ�Ʒ�����.
*/
class CProdCycle: public CProdBillCycleImpl, private CBillCycle		//! ע: ���밴���˳��̳�!
{
public:
	CProdCycle(const int64 doneCode,
		const MXdr::SRateProdId& prodOrderInfo, const MXdr::SRateProdBillCycle& prodBillCycle,
		const CBSDateTime& llProcessDate,
		const SProrateRule& prorate_rules
	);
	~CProdCycle()
	{
	}
public:
	void cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&);
	const int32 get_cycleNum() const;
private:
	const CBSDateTime m_llProcessDate;
	//! ��Ʒ�Ʒ����ڱ��ǰ�ۻ���������
	const int32 m_iPreCycleCount;
};

CProdCycle::CProdCycle(const int64 doneCode,
	const MXdr::SRateProdId& prodOrderInfo, const MXdr::SRateProdBillCycle& prodBillCycle,
	const CBSDateTime& llProcessDate,
	const SProrateRule& prorate_rules)
	: m_llProcessDate(llProcessDate)
	, CBillCycle(getICleanCycle(), prodBillCycle.get_firstCycleDateTime(), prodBillCycle.get_validDateTime(), prodBillCycle.get_expireDateTime())
	, CProdBillCycleImpl(doneCode, prodOrderInfo, CleanCycleFactory::toEnumCycleType(prodBillCycle.get_cycleType()), prodBillCycle.get_cycleUnit(), prorate_rules)
	, m_iPreCycleCount(prodBillCycle.get_cycleCount() < 1 ? 1 : prodBillCycle.get_cycleCount())
{
}

void CProdCycle::cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&)
{
	//���⴦�����NoCycle��Ʒ������������ʧЧʱ��Ϊ����ʱ��
	if (!CleanCycleFactory::isValidToCycleType(CProdBillCycleImpl::m_pCycle->get_CyleType()))
	{
		m_CycleNum =1;
		llRealValidDate = m_cycleValidDate;
		llRealValidDate.addSecs(0 - 3600*m_iTimezoneOffset);
		llRealExpireDate = m_cycleExpireDate;
		llRealExpireDate.addSecs(0 - 3600*m_iTimezoneOffset);

		llHopeValidDate = llRealValidDate;
		llHopeExpireDate = llRealExpireDate;
	}else
	{
		m_CycleNum = CBillCycle::cal_validAndExpireDate(m_llProcessDate, llRealValidDate, llRealExpireDate, llHopeValidDate, llHopeExpireDate, m_iTimezoneOffset);
	}

}

const int32 CProdCycle::get_cycleNum() const
{
	return m_iPreCycleCount + m_CycleNum - 1;
}

/*
	����.
*/
class CAccountCycle: public CProdBillCycleImpl, private CBillCycle	//! ע: ���밴���˳��̳�!
{
public:
	CAccountCycle(const int64 doneCode,
		const MXdr::SAcctBillCycle& acctBillCycle, const MXdr::SRateProdId& prodOrderInfo,
		const CBSDateTime& llProcessDate,
		const SProrateRule& prorate_rules
	);
	~CAccountCycle()
	{
	}
public:
	void cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&);
	const int32 get_cycleNum() const;
private:
	const CBSDateTime m_llProcessDate;
};

CAccountCycle::CAccountCycle(const int64 doneCode,
	const MXdr::SAcctBillCycle& acctBillCycle, const MXdr::SRateProdId& prodOrderInfo,
	const CBSDateTime& llProcessDate,
	const SProrateRule& prorate_rules): m_llProcessDate(llProcessDate), CBillCycle(getICleanCycle(), acctBillCycle.get_firstBillDate(), acctBillCycle.get_billValidDate(), acctBillCycle.get_billExpireDate()), CProdBillCycleImpl(doneCode,prodOrderInfo, CleanCycleFactory::toEnumCycleType(acctBillCycle.get_cycleType()), acctBillCycle.get_cycleUnit(), prorate_rules)
{
}

void CAccountCycle::cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&)
{
	m_CycleNum = CBillCycle::cal_validAndExpireDate(m_llProcessDate, llRealValidDate, llRealExpireDate, llHopeValidDate, llHopeExpireDate, m_iTimezoneOffset);

	llRealValidDate = IsRefProdValidDate ? (llRealValidDate > m_ProdValidDate ? llRealValidDate : m_ProdValidDate) : llRealValidDate;
	llRealExpireDate = IsRefProdExpireDate ? (llRealExpireDate < m_ProdExpireDate ? llRealExpireDate : m_ProdExpireDate) : llRealExpireDate;

//	if (llRealValidDate > llRealExpireDate)
//	{
//		char szX[256];
//		snprintf(szX, sizeof(szX), "The instance information(MXdr::SRateProdId) of this product[%lld] has expired in the current time[%lld].", m_llDoneCode, m_llProcessDate);
//		throw CExpX(szX,3399);
//	}
}

const int32 CAccountCycle::get_cycleNum() const
{
	if (m_ProdValidDate > m_cycleValidDate)
	{
		CBSDateTime llValidDate, llExpireDate, llHopeValidDate, llHopeExpireDate;
		const int32 cycleNum = CBillCycle::cal_validAndExpireDate(m_ProdValidDate, llValidDate, llExpireDate, llHopeValidDate, llHopeExpireDate, 0);
		return m_CycleNum - cycleNum + 1;
	}
	else
	{
		return m_CycleNum;
	}
}

/*
	�����ݶ���.
*/
class CConfigCycle: public CProdBillCycleImpl
{
public:
	CConfigCycle(const int64 doneCode,
		const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules,
		const MXdr::SRateProdId& prodOrderInfo,
		const CBSDateTime& llProcessDate, const CBSDateTime& llBaseDate
	);
	~CConfigCycle()
	{
	}
public:
	void cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&);
private:
	const CBSDateTime m_llProcessDate;
	const CBSDateTime m_llBaseDate;
};

CConfigCycle::CConfigCycle(const int64 doneCode,
	const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules,
	const MXdr::SRateProdId& prodOrderInfo,
	const CBSDateTime& llProcessDate, const CBSDateTime& llBaseDate): m_llBaseDate(llBaseDate), m_llProcessDate(llProcessDate), CProdBillCycleImpl(doneCode, prodOrderInfo, cycleType, cycleUnit, prorate_rules)
{
}

void CConfigCycle::cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&)
{
	m_CycleNum = getICleanCycle()->cal_validAndExpireDate(m_llBaseDate, m_llProcessDate, llHopeValidDate, llHopeExpireDate, m_iTimezoneOffset);

	llRealValidDate = IsRefProdValidDate ? (llHopeValidDate > m_ProdValidDate ? llHopeValidDate : m_ProdValidDate) : llHopeValidDate;
	llRealExpireDate = IsRefProdExpireDate ? (llHopeExpireDate < m_ProdExpireDate ? llHopeExpireDate : m_ProdExpireDate) : llHopeExpireDate;
}

/*
	�����ݶ���, ���������ۿ�ֵ��BaseDateָ��.
*/
class CConfigCycleEx: public CProdBillCycle
{
public:
	CConfigCycleEx(const int64 doneCode,
		const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules,
		const MXdr::SRateProdId& prodOrderInfo,
		const CBSDateTime& llProcessDate, const CBSDateTime& llBaseDate
	);
	~CConfigCycleEx()
	{
		delete _configCycle;
	}
public:
	void cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&);
	const float get_discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&);
	const int32 get_cycleNum() const;
private:
	const CConfigCycle* _configCycle;
	const CBSDateTime m_llBaseDate;
};

CConfigCycleEx::CConfigCycleEx(const int64 doneCode,
	const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules,
	const MXdr::SRateProdId& prodOrderInfo,
	const CBSDateTime& llProcessDate, const CBSDateTime& llBaseDate): m_llBaseDate(llBaseDate), _configCycle(new CConfigCycle(doneCode, cycleType, cycleUnit, prorate_rules, prodOrderInfo, llProcessDate, llBaseDate))
{
}

void CConfigCycleEx::cal_validAndExpireDate(CBSDateTime& llRealValidDate, CBSDateTime& llRealExpireDate, CBSDateTime& llHopeValidDate, CBSDateTime& llHopeExpireDate, const bool IsRefProdExpireDate, const bool IsRefProdValidDate) const throw (const CExpX&)
{
	_configCycle->cal_validAndExpireDate(llRealValidDate, llRealExpireDate, llHopeValidDate, llHopeExpireDate, IsRefProdExpireDate, IsRefProdValidDate);
}

const float CConfigCycleEx::get_discount(const CBSDateTime& llRealValidDate, const CBSDateTime& llRealExpireDate, const CBSDateTime& llHopeValidDate, const CBSDateTime& llHopeExpireDate) const throw (const CExpX&)
{
	return _configCycle->get_discount((1 == get_cycleNum() ? m_llBaseDate : llRealValidDate), llRealExpireDate, llHopeValidDate, llHopeExpireDate);
}

const int32 CConfigCycleEx::get_cycleNum() const
{
	return _configCycle->get_cycleNum();
}

//!
void CBillCycleFactory::checkProd(const int64 llDoneCodeIn,
		const CBSDateTime& processTime,
		const RefModeType refModeType,
		const MXdr::SRateProdIdMap& rateProdMap,
		int64 &llDoneCodeOut,
		MXdr::SRateProdId& rateProdInfo) throw (const CExpX&)
{	
	if(-1==llDoneCodeIn||refModeType==eMainProdCycle)
	{
		for(MXdr::SRateProdIdMap::const_iterator iter=rateProdMap.begin();iter!=rateProdMap.end();++iter)
		{
			if(1==iter->second.get_prodType())
			{
				llDoneCodeOut=iter->first;
				rateProdInfo=iter->second;
				return;
			}
		}
	}
	
	llDoneCodeOut=llDoneCodeIn;
	MXdr::SRateProdIdMap::const_iterator rateProd_itr = rateProdMap.find(llDoneCodeOut);//��ȡ�ò�Ʒʵ���Ķ�����Ϣ.
	if (rateProd_itr == rateProdMap.end())
	{
		CExpX e(622019);
		e << llDoneCodeOut;
		RATING_TRACES(e.what());
		throw e;
		//_ERROE_MSG   object: productId ?, error msg: The productId[?] can not find product order info in the xdr, suggest: check product list in xdr.
	}

	rateProdInfo = rateProd_itr->second;
	if (processTime < rateProdInfo.get_validDateTime() || processTime >= rateProdInfo.get_expireDateTime())
	{
		throw CExpX (622020)  << processTime << llDoneCodeOut<< rateProdInfo.get_validDateTime() << rateProdInfo.get_expireDateTime();
		//_ERROE_MSG   object: DoneCode ?, error msg: DoneCode [?] processTime[?] should between Product ValidDate[?] expireDate [?], suggest: check product's valid date in xdr.
	}
}

CBSDateTime CBillCycleFactory::checkActivation(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&)
{
	const MXdr::StringMap& resMap = subCommon.get_reserveFields();
	const aistring strKey("FIRST_ACTIVATE_TIME");
	MXdr::StringMap::const_iterator Iter = resMap.find(strKey);

	if (Iter == resMap.end())
	{
		throw CExpX(622021)<<strKey.c_str()<<doneCode;
		//_ERROE_MSG   object: Activation Time ?, error msg: The User Activation Time[KEY: ?] can not find in the xdr, DoneCode [?], suggest: add The activation time in xdr.
	}
	const CBSDateTime llBaseDate(Iter->second.c_str());
	if (llBaseDate.isValid())
	{
		throw CExpX(622022) << llBaseDate<< doneCode;
		//_ERROE_MSG   object: Activation Time ?, error msg: User Activation Time[?] is invalid, DoneCode [?], suggest: check Activation Time in xdr.
	}
	if (llBaseDate > processTime)
	{
		throw CExpX(622023)<< processTime<< llBaseDate<< doneCode;
		//_ERROE_MSG   processTime[?] should be equal or greater than User Activation Time[?], DoneCode [?], suggest: check user activation time
	}
	if (llBaseDate > rateProdInfo.get_validDateTime())
	{
		throw CExpX(622024)<<llBaseDate<< rateProdInfo.get_validDateTime()<< doneCode;
		//_ERROE_MSG   User Activation Time[?] should be equal or less than Product ValidDate[?], DoneCode [?], suggest: check user activation time
	}

	return llBaseDate;
}

CBSDateTime CBillCycleFactory::checkUserAccess(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&)
{
	const CBSDateTime llBaseDate = subCommon.get_userBaseInfo().get_createDate();
	if (!llBaseDate.isValid())
	{
		throw CExpX(622025)<< llBaseDate<< doneCode;
		//_ERROE_MSG   User Access Time[?] is invalid, DoneCode [?], suggest: check user access time
	}
	if (llBaseDate > processTime)
	{
		throw CExpX(622026)<< processTime<< llBaseDate<< doneCode;
		//_ERROE_MSG   processTime[?] should be equal or greater than User Access Time[?], DoneCode [?], suggest: check user access time
	}
	if (llBaseDate > rateProdInfo.get_validDateTime())
	{
		throw CExpX(622027)<< llBaseDate<< rateProdInfo.get_validDateTime()<< doneCode;
		//_ERROE_MSG   User Access Time[?] should be equal or less than Product ValidDate[?], DoneCode [?], suggest: check user access time
	}

	return llBaseDate;
}

void CBillCycleFactory::checkAcctBillCycle(const MXdr::SAcctBillCycle& acctBillCycle, const CBSDateTime& processTime) throw (const CExpX&)
{
	const CBSDateTime cycleExpireDate = acctBillCycle.get_billExpireDate();
	const CBSDateTime cycleValidDate = acctBillCycle.get_billValidDate();

	if (processTime < cycleValidDate || processTime >= cycleExpireDate)
	{
		throw CExpX(622028)<< processTime<< cycleValidDate<< cycleExpireDate;
		//_ERROE_MSG   processTime[?] should between Account bill cycle ValidDate[?] expireDate [?], suggest: check product bill cycle date
	}
	if (!CleanCycleFactory::isValidToCycleType(acctBillCycle.get_cycleType()))
	{
    	throw CExpX(622029)<< acctBillCycle.get_cycleType()<< acctBillCycle.get_cycleUnit();
		//_ERROE_MSG   Account bill cycle is invalid: CYCLE_TYPE[?] CYCLE_UNIT [?], suggest: check product bill cycle type
	}
	if (cycleValidDate >= cycleExpireDate)
	{
		throw CExpX(622030)<< cycleValidDate<< cycleExpireDate;
		//_ERROE_MSG   Account bill cycle ValidDate[?] should be less than expireDate [?], suggest: check product bill cycle date
	}
}

const MXdr::SAcctBillCycle& CBillCycleFactory::checkAcctBillCycle(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&)
{
	const MXdr::SAcctBillCycle& acctBillCycle = subCommon.get_userBaseInfo().get_acctBillCycle();
	try{
		checkAcctBillCycle(acctBillCycle, processTime);
	}catch(const CExpX& exp){
		throw CExpX(622099)<< doneCode<< exp.get_errno()<< exp.what();
		//_ERROE_MSG   DoneCode [?], base err code ?, err msg:?, suggest: check product bill cycle
	}

	return acctBillCycle;
}

const MXdr::SRateProdBillCycle& CBillCycleFactory::checkProdBillCycle(const MXdr::SRateProdId& rateProdInfo, const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon) throw (const CExpX&)
{
	const MXdr::SRateProdBillCycleMap& rateProdBillCycleMap = subCommon.get_rateProdBillCycle();			//������Ʒ�Ʒ�����
	//��ȡ�ò�Ʒ��Ʒ�Ʒ�����
	MXdr::SRateProdBillCycleMap::const_iterator billCycle_itr = rateProdBillCycleMap.find(doneCode);
	if (billCycle_itr == rateProdBillCycleMap.end())
	{
		throw CExpX(622031) << doneCode;
		//_ERROE_MSG   The productId[?] can not find Product bill cycle info in the xdr, suggest: check product bill cycle
	}

	const MXdr::SRateProdBillCycle& rateProdBillCycle = billCycle_itr->second;
	
	checkProdBillCycle(doneCode,rateProdInfo,rateProdBillCycle,processTime);

	return rateProdBillCycle;
}

void CBillCycleFactory::checkProdBillCycle(const int64 doneCode, const MXdr::SRateProdId& rateProdInfo, const MXdr::SRateProdBillCycle& rateProdBillCycle, const CBSDateTime& processTime ) throw (const CExpX&)
{
	const CBSDateTime cycleExpireDate = rateProdBillCycle.get_expireDateTime();
	const CBSDateTime cycleValidDate = rateProdBillCycle.get_validDateTime();
	if (processTime < cycleValidDate || processTime >= cycleExpireDate)
	{
		throw CExpX(622032)<< processTime<< doneCode<< cycleValidDate<< cycleExpireDate;
		//_ERROE_MSG   DoneCode [?] processTime[?] should between Product bill cycle ValidDate[?] expireDate [?], suggest: check product bill cycle date.
	}

/*HU ���󣬶���NO CYCLE��Ʒ���������Ϳ�������Ч��
	if (!CleanCycleFactory::isValidToCycleType(rateProdBillCycle.get_cycleType()))
	{
		throw CExpX(622033)<< doneCode<< rateProdBillCycle.get_cycleType()<< rateProdBillCycle.get_cycleUnit();
		//_ERROE_MSG   DoneCode [?]'s Product bill cycle is invalid: CYCLE_TYPE[?] CYCLE_UNIT [?], suggest: check product bill cycle type
    }
*/
	if (cycleValidDate >= cycleExpireDate)
	{
		throw CExpX(622034)<< cycleValidDate<< doneCode<< cycleExpireDate;
		//_ERROE_MSG   DoneCode [?] Product bill cycle ValidDate[?] should be less than expireDate [?], suggest: check product validdate and bill cycle validdate
	}
	if (cycleValidDate < rateProdInfo.get_validDateTime())
	{
		throw CExpX(622035)<< cycleValidDate<< doneCode<< rateProdInfo.get_validDateTime();
		//_ERROE_MSG   DoneCode [?]'s Product bill cycle ValidDate[?] should be equal or greater than Product ValidDate[?], suggest: check product bill cycle date.
	}
	if (cycleExpireDate > rateProdInfo.get_expireDateTime())
	{
		throw CExpX(622036)<< cycleExpireDate<< doneCode<< rateProdInfo.get_expireDateTime();
		//_ERROE_MSG   DoneCode [?]'s Product bill cycle expireDate[?] should be equal or less than Product expireDate[?], suggest: check product expiredate and bill cycle expiredate
	}

}

const CProdBillCycle* CBillCycleFactory::get_billCycleWithDiscountAndConfig(const int64 doneCode, const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const RefModeType refModeType, const CycleType cycleType, const int32 cycleUnit, const SProrateRule& prorate_rules) throw (const CExpX&)
{
	MXdr::SRateProdId rateProdInfo;
	int64 llDoneCode=0;
	//doneCodeΪ-1 ���� refModeTypeΪeMainProdCycle��rateProdInfo��llDoneCodeΪ����Ʒ����Ϣ
	checkProd(doneCode, processTime, refModeType,subCommon.get_rateProdId(),llDoneCode,rateProdInfo);
	//���⴦�����ڲο���Ʒ���ڣ�ͬʱ��Ʒ���ڲο�����ʱ����ʱҵ������������Ʒ�Ʒ����ڵ�prod_bill_cycle
	//���Ե��Ҳ���prod_bill_cyleʱ��ǿ�ƽ�refMode��ֵΪeAccoutCycle��
	//����ο���Ʒ���ڵĳ����£�δ�ҵ���Ʒ���ڣ�ǿ������Ϊ�ο�����
	RefModeType newRefModeType = refModeType;
	changeRefModeType(llDoneCode,subCommon.get_rateProdBillCycle(),newRefModeType);
	//!
	CProdBillCycle* pBillCycle = NULL;
	switch (newRefModeType)
	{
	case eAccoutCycle:
		{//! ��������.
			const MXdr::SAcctBillCycle& acctBillCycle = checkAcctBillCycle(llDoneCode, processTime, subCommon);

			pBillCycle = new CAccountCycle(llDoneCode, acctBillCycle, rateProdInfo, processTime, prorate_rules);
		}
		break;
	case eProdCycle:
	case eMainProdCycle:
		{//! ���ݲ�Ʒ�Ʒ�����.
			const MXdr::SRateProdBillCycle& rateProdBillCycle = checkProdBillCycle(rateProdInfo, llDoneCode, processTime, subCommon);

			pBillCycle = new CProdCycle(llDoneCode, rateProdInfo, rateProdBillCycle, processTime, prorate_rules);
		}
		break;
	case eXCBasedOnProd:
		{
			const CBSDateTime llBaseDate = rateProdInfo.get_validDateTime();

			pBillCycle = new CConfigCycle(llDoneCode, cycleType, cycleUnit, prorate_rules, rateProdInfo, processTime, llBaseDate);
		}
		break;
	case eXCBasedOnActivation:
		{
			const CBSDateTime llBaseDate = checkActivation(rateProdInfo, llDoneCode, processTime, subCommon);

			pBillCycle = new CConfigCycle(llDoneCode, cycleType, cycleUnit, prorate_rules, rateProdInfo, processTime, llBaseDate);
		}
		break;
	case eXCBasedOnUserAccess:
		{
			const CBSDateTime llBaseDate = checkUserAccess(rateProdInfo, llDoneCode, processTime, subCommon);

			pBillCycle = new CConfigCycle(llDoneCode, cycleType, cycleUnit, prorate_rules, rateProdInfo, processTime, llBaseDate);
		}
		break;
	case eXCBasedOnActivationEx:
		{
			const CBSDateTime llBaseDate = checkActivation(rateProdInfo, llDoneCode, processTime, subCommon);

			pBillCycle = new CConfigCycleEx(llDoneCode, cycleType, cycleUnit, prorate_rules, rateProdInfo, processTime, llBaseDate);
		}
		break;
	default:
		{
			throw CExpX(622037)<< llDoneCode;
			//_ERROE_MSG   The instance information(MXdr::SRateProdId) of this product[?] is Error: Invalid CYCLE_REF_MODE's value, suggest: check product's CYCLE_REF_MODE
		}
		break;
	}

	return pBillCycle;
}

//! ������.
const CProdBillCycle* CBillCycleFactory::get_billCycleWithDiscountAndConfig(
	const int64 doneCode, const CBSDateTime& processTime ,
	const MXdr::SSubCommon& subCommon,
	const int32 cycleRefMode,
	const CycleType cycleType, const int32 cycleUnit,
	const SProrateRule& prorate_rules) throw (const CExpX&)
{
	return get_billCycleWithDiscountAndConfig(doneCode, processTime, subCommon, toEnumRefModeType(cycleRefMode), cycleType, cycleUnit, prorate_rules);
}

const CProdBillCycle* CBillCycleFactory::get_billCycleWithConfig(const int64 doneCode,const CBSDateTime& processTime , const MXdr::SSubCommon& subCommon, const int32 cycleRefMode, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&)
{
//	SProrateRule prorate_rules;//Ĭ����(0,0)
	return get_billCycleWithDiscountAndConfig(doneCode, processTime, subCommon, toEnumRefModeType(cycleRefMode), cycleType, cycleUnit, SProrateRule());
}

const CProdBillCycle* CBillCycleFactory::get_billCycleWithoutConfig(const int64 doneCode, const CBSDateTime& processTime ,const MXdr::SSubCommon &subCommon, const int32 cycleRefMode) throw (const CExpX&)
{
//	SProrateRule prorate_rules;//Ĭ����(0,0)
	return get_billCycleWithDiscountAndConfig(doneCode, processTime, subCommon, toEnumRefModeType(cycleRefMode), eInvalidCycle, 1, SProrateRule());
}

//!
int32 CBillCycleFactory::cal_acctBillCycle(const MXdr::SAcctBillCycle& acctBillCycle, const CBSDateTime& processTime, CBSDateTime& llValidDate, CBSDateTime& llExpireDate) throw (const CExpX&)
{
	checkAcctBillCycle(acctBillCycle, processTime);

	const ICleanCycle* _pCycle = CleanCycleFactory::get_cleanCycle(CleanCycleFactory::toEnumCycleType(acctBillCycle.get_cycleType()), acctBillCycle.get_cycleUnit());
	AISTD auto_ptr<const ICleanCycle> pCycle(_pCycle);

	const CBillCycle billCycle(pCycle.get(), acctBillCycle.get_firstBillDate(), acctBillCycle.get_billValidDate(), acctBillCycle.get_billExpireDate());
	try{
		CBSDateTime llHopeValidDate, llHopeExpireDate;
		return billCycle.cal_validAndExpireDate(processTime, llValidDate, llExpireDate, llHopeValidDate, llHopeExpireDate, 0);
	}catch(...){
		throw;
	}
}

int32 CBillCycleFactory::cal_prodBillCycle( const int64 doneCode,const MXdr::SRateProdId& rateProdInfo, const MXdr::SRateProdBillCycle& rateProdCycle, const CBSDateTime& processTime, CBSDateTime& llValidDate, CBSDateTime& llExpireDate ) throw (const CExpX&)
{
	checkProdBillCycle(doneCode,rateProdInfo,rateProdCycle,processTime);
	const ICleanCycle* _pCycle = CleanCycleFactory::get_cleanCycle(CleanCycleFactory::toEnumCycleType(rateProdCycle.get_cycleType()), rateProdCycle.get_cycleUnit());
	AISTD auto_ptr<const ICleanCycle> pCycle(_pCycle);

	const CBillCycle billCycle(pCycle.get(), rateProdCycle.get_firstCycleDateTime(), rateProdCycle.get_validDateTime(), rateProdCycle.get_expireDateTime());
	try{
		CBSDateTime llHopeValidDate, llHopeExpireDate;
		return billCycle.cal_validAndExpireDate(processTime, llValidDate, llExpireDate, llHopeValidDate, llHopeExpireDate, 0);
	}catch(...){
		throw;
	}
}
CBSDateTime CBillCycleFactory::cal_expireDate(const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int64 doneCode, const RefModeType refModeType, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&)
{
	MXdr::SRateProdId rateProdInfo;
	int64 llDoneCode=0;
	checkProd(doneCode, processTime, refModeType,subCommon.get_rateProdId(),llDoneCode,rateProdInfo);
	RefModeType newRefModeType = refModeType;
	changeRefModeType(llDoneCode,subCommon.get_rateProdBillCycle(),newRefModeType);
	const CProdBillCycle* pBillCycle = NULL;
	switch (refModeType)
	{
	case eAccoutCycle:
		{//! ��������.
			const MXdr::SAcctBillCycle& acctBillCycle = checkAcctBillCycle(llDoneCode, processTime, subCommon);

			pBillCycle = new CAccountCycle(llDoneCode, acctBillCycle, rateProdInfo, processTime, SProrateRule());
		}
		break;
	case eProdCycle:
	case eMainProdCycle:
		{//! ���ݲ�Ʒ�Ʒ�����.
			const MXdr::SRateProdBillCycle& rateProdBillCycle = checkProdBillCycle(rateProdInfo, llDoneCode, processTime, subCommon);

			pBillCycle = new CProdCycle(llDoneCode, rateProdInfo, rateProdBillCycle, processTime, SProrateRule());
		}
		break;
	default:
		{
			pBillCycle = new CConfigCycle(llDoneCode, cycleType, cycleUnit, SProrateRule(), rateProdInfo, processTime, processTime);
		}
		break;
	}
	ASSERT(pBillCycle != NULL);

	AISTD auto_ptr<const CProdBillCycle> _pBillCycle(pBillCycle);
	try{
		CBSDateTime llValidDate, llExpireDate;
		_pBillCycle->cal_validAndExpireDate(llValidDate, llExpireDate, false, false);

		return llExpireDate;
	}catch(...){
		throw;
	}
}

CBSDateTime CBillCycleFactory::cal_expireDate(const CBSDateTime& processTime, const MXdr::SSubCommon& subCommon, const int64 doneCode, const int32 cycleRefMode, const CycleType cycleType, const int32 cycleUnit) throw (const CExpX&)
{
	return cal_expireDate(processTime, subCommon, doneCode, toEnumRefModeType(cycleRefMode), cycleType, cycleUnit);
}

void CBillCycleFactory::changeRefModeType( const int64 llDoneCodeIn,const MXdr::SRateProdBillCycleMap& rateProdCycleMap,RefModeType& refModeTypeInOut ) throw (const CExpX&)
{
	if(eProdCycle==refModeTypeInOut||eMainProdCycle==refModeTypeInOut)
	{
		MXdr::SRateProdBillCycleMap::const_iterator itr = rateProdCycleMap.find(llDoneCodeIn);
		if (itr ==  rateProdCycleMap.end())
		{
			RATING_TRACES("Change eProdCycle eMainProdCycle  to eAccoutCycle because of no billcyle in xdr");
			refModeTypeInOut = eAccoutCycle;
		}
	}
	
}
END_NAMESPACE
