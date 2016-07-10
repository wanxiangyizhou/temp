/* $Id: ProrateDeductRuleFactory.h 311559 2013-04-17 07:54:28Z qijf $  */
#ifndef _PRORATE_DEDUCT_RULE_FACTORY_H_
#define _PRORATE_DEDUCT_RULE_FACTORY_H_
#include <IFactory.h>

BEGIN_NAMESPACE

class IProrateDeductRule;
class CProrateDeductRule;

class CProrateDeductRuleFactory : public IFactory
{
public:
	CProrateDeductRuleFactory(void);
	~CProrateDeductRuleFactory(void);

public:
	//��ʼ���ӿ�
	void init(const xc::CSnapshot& cSnapShot)throw (const CExpX&);

	//���¼���������
	void clear();

	const IProrateDeductRule& get_prorateDeductRuleInterface()const;


private:
	CProrateDeductRuleFactory(const CProrateDeductRuleFactory&);
	CProrateDeductRuleFactory& operator=(const CProrateDeductRuleFactory&);

private:

	CProrateDeductRule* m_pProrateDeductRule;
};

END_NAMESPACE

#endif
