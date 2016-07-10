/* $Id: ProrateDeductRuleFactory.cpp 311559 2013-04-17 07:54:28Z qijf $  */
#include "ProrateDeductRuleFactory.h"
#include "ProrateDeductRule.h"
BEGIN_NAMESPACE

CProrateDeductRuleFactory::CProrateDeductRuleFactory(void)
{
	m_pProrateDeductRule = new CProrateDeductRule();
}

CProrateDeductRuleFactory::~CProrateDeductRuleFactory(void)
{
	delete m_pProrateDeductRule;
	m_pProrateDeductRule = NULL;
}
//��ʼ���ӿ�
void CProrateDeductRuleFactory::init(const xc::CSnapshot& cSnapShot)throw (const CExpX&)
{
	m_pProrateDeductRule->init(cSnapShot);
}

//���¼���������
void CProrateDeductRuleFactory::clear()
{
    m_pProrateDeductRule->clear();
}

const IProrateDeductRule& CProrateDeductRuleFactory::get_prorateDeductRuleInterface()const
{
	return *m_pProrateDeductRule;
}

END_NAMESPACE
