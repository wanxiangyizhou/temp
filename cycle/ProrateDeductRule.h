/* $Id: ProrateDeductRule.h 311559 2013-04-17 07:54:28Z qijf $  */
#ifndef _PRORATE_DEDUCT_RULE_H_
#define _PRORATE_DEDUCT_RULE_H_
#include "IProrateDeductRule.h"
#include <charging_exp_x.h>
#include "aivector.h"
#include <xqueryrating_xc.h>

BEGIN_NAMESPACE

//����Ʒ�۷ѹ���
//struct SDeductRuleKey
//{
//	int32 offer_prod_id;
//	int32 billing_type;
//	int32 main_promotion;	//����Ʒ������ƷID 
//	
//	bool operator <(const SDeductRuleKey& other) const
//	{
//		if(offer_prod_id < other.offer_prod_id)
//		{
//			return true;
//		}
//		else if(offer_prod_id > other.offer_prod_id)
//		{
//			return false;
//		}
//		
//		if(billing_type < other.billing_type)
//		{
//			return true;
//		}
//		else if(billing_type > other.billing_type)
//		{
//			return false;
//		}
//		
//		if(main_promotion == -1 
//			|| other.main_promotion == -1
//			|| main_promotion > other.main_promotion)
//		{
//			return false;
//		}
//		else
//		{
//			return true;
//		}		
//	}
//};

struct SCompositeDeductRule
{
	int32 offer_prod_id;				//����Ʒ���
	int32 deduct_flag;					//������ÿ۷ѷ�ʽ	0--Ԥ��	1--��� 2--����
	int32 resource_flag;				//��ʾ�����Դ�����Ƿ������̷ѿۼ�,	0��������(Ĭ��)	1������
//	int32 rent_deduct_rule_id;			//�̷ѿۼ�ʧ�ܹ���ID
	int32 prorate_deduct_rule_id;		//���¿۷��۰����ID
	int32 main_promotion;	//����Ʒ������ƷID 
};
typedef aivector<SCompositeDeductRule> SCompositeDeductRuleList;
//����int64Ϊoffer_prod_id+billtype��������,����ƷID��Ԥ/�󸶷�����
typedef aimap<int64,SCompositeDeductRuleList> SCompositeDeductRuleMap; 

//���¿۷��۰������ϸ
struct SProrateDeductRuleDtl
{
	int32 prorate_deduct_rule_id;		//���¿۷��۰����ID
	int32 rule_class;					//0��ͨ�� 1�������Դ 2���ۻ���� 3���̶�����
	int32 effect_type;					//0�������ڲ�Ʒ��Чʱ 1�������ڲ�ƷʧЧʱ
	int32 prorate_type;					//0�����ۿ� 1����ʾ�������ۿ� 2����ʾ����������������
	
};
typedef aimap<int32,aivector<SProrateDeductRuleDtl> > SProrateDeductRuleDtlMap; //����int32Ϊprorate_deduct_rule_id,���¿۷��۰����ID


typedef aimap<int32,SProrateRule> SProrateRuleMap; //����int32Ϊprorate_deduct_rule_id,���¿۷��۰����ID


class CProrateDeductRule : public IProrateDeductRule
{
	//enum rRuleClass{rFreeres = 1u, rAccumulate = 2u};
	
public:
	CProrateDeductRule();
	~CProrateDeductRule();
public:
	//��ʼ���ӿ�
	void init(const xc::CSnapshot& cSnapShot)throw (const CExpX&);
	
	//���¼���������
	void clear();
public:	
	//�ӿ�. 
	//��ȡ���������Դ�İ��¿۷��۰������nProrateType��effectType	
	void get_freeResProrateRule(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion, SProrateRule& prorate_rule)const;	
	
	//��ȡ�����ۻ���صİ��¿۷��۰������nProrateType��effectType
	void get_accumulateProrateRule(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion, SProrateRule& prorate_rule )const;

	//��ȡ�����Դ����ʱԤ�ۡ���ۡ���������
	//0:Ԥ��;1:���;2:����
	int32 get_deductFlag(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion)const throw (const CExpX&);

	//�����Դ�����Ƿ������̷ѿۼ�����
	int32 get_resourceFlag(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion)const throw (const CExpX&);
private:
	//����Ʒ�۷ѹ���
	void init_compositeDeductRule(const xc::CSnapshot& cSnapShot);
		
	//���¿۷��۰������ϸ
	const SProrateDeductRuleDtlMap init_prorateDeductRule(const xc::CSnapshot& cSnapShot);
	
	//���˳�������۰����У��һ��prorate_deduct_rule_id����һ��prorate_type��
	//���ó�ÿ��prorate_deduct_rule_id��Ӧ���۰������ϸ
	void check_prorateDeductRule(const SProrateDeductRuleDtlMap& ProrateDeductRuleDtl,const int32 &ruleClass, 
															SProrateRuleMap &prorateRuleMap) throw (const CExpX&);
	
	bool get_compositeDeductRule(
		const int32 offer_prod_id,
		const int32 bill_type,
		const int32 main_promotion,
		SCompositeDeductRule &sCompositeDeductRule) const;
private:
	SCompositeDeductRuleMap m_sCompositeDeductRule;		//����Ʒ�۷ѹ���
	
	SProrateRuleMap m_sFreeResProrateRuleMap;		//�����Դ���۰����
	SProrateRuleMap m_sAccumulateProrateRuleMap;		//�ۼ������۰����
};

END_NAMESPACE

#endif
