/* $Id: ProrateDeductRule.h 311559 2013-04-17 07:54:28Z qijf $  */
#ifndef _PRORATE_DEDUCT_RULE_H_
#define _PRORATE_DEDUCT_RULE_H_
#include "IProrateDeductRule.h"
#include <charging_exp_x.h>
#include "aivector.h"
#include <xqueryrating_xc.h>

BEGIN_NAMESPACE

//销售品扣费规则
//struct SDeductRuleKey
//{
//	int32 offer_prod_id;
//	int32 billing_type;
//	int32 main_promotion;	//主产品的销售品ID 
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
	int32 offer_prod_id;				//销售品编号
	int32 deduct_flag;					//账务费用扣费方式	0--预扣	1--后扣 2--不扣
	int32 resource_flag;				//表示免费资源生成是否依赖固费扣减,	0：不依赖(默认)	1：依赖
//	int32 rent_deduct_rule_id;			//固费扣减失败规则ID
	int32 prorate_deduct_rule_id;		//半月扣费折半规则ID
	int32 main_promotion;	//主产品的销售品ID 
};
typedef aivector<SCompositeDeductRule> SCompositeDeductRuleList;
//其中int64为offer_prod_id+billtype联合主键,销售品ID，预/后付费属性
typedef aimap<int64,SCompositeDeductRuleList> SCompositeDeductRuleMap; 

//半月扣费折半规则明细
struct SProrateDeductRuleDtl
{
	int32 prorate_deduct_rule_id;		//半月扣费折半规则ID
	int32 rule_class;					//0：通用 1：免费资源 2、累积相关 3、固定费用
	int32 effect_type;					//0：作用于产品生效时 1：作用于产品失效时
	int32 prorate_type;					//0：无折扣 1：表示半周期折扣 2：表示按天数或秒数折算
	
};
typedef aimap<int32,aivector<SProrateDeductRuleDtl> > SProrateDeductRuleDtlMap; //其中int32为prorate_deduct_rule_id,半月扣费折半规则ID


typedef aimap<int32,SProrateRule> SProrateRuleMap; //其中int32为prorate_deduct_rule_id,半月扣费折半规则ID


class CProrateDeductRule : public IProrateDeductRule
{
	//enum rRuleClass{rFreeres = 1u, rAccumulate = 2u};
	
public:
	CProrateDeductRule();
	~CProrateDeductRule();
public:
	//初始化接口
	void init(const xc::CSnapshot& cSnapShot)throw (const CExpX&);
	
	//重新加载数据用
	void clear();
public:	
	//接口. 
	//获取属于免费资源的半月扣费折半规则中nProrateType和effectType	
	void get_freeResProrateRule(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion, SProrateRule& prorate_rule)const;	
	
	//获取属于累积相关的半月扣费折半规则中nProrateType和effectType
	void get_accumulateProrateRule(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion, SProrateRule& prorate_rule )const;

	//获取免费资源生成时预扣、后扣、不扣属性
	//0:预扣;1:后口;2:不扣
	int32 get_deductFlag(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion)const throw (const CExpX&);

	//免费资源生成是否依赖固费扣减属性
	int32 get_resourceFlag(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion)const throw (const CExpX&);
private:
	//销售品扣费规则
	void init_compositeDeductRule(const xc::CSnapshot& cSnapShot);
		
	//半月扣费折半规则明细
	const SProrateDeductRuleDtlMap init_prorateDeductRule(const xc::CSnapshot& cSnapShot);
	
	//过滤出所需的折半规则，校验一个prorate_deduct_rule_id对于一个prorate_type；
	//并得出每个prorate_deduct_rule_id对应的折半规则明细
	void check_prorateDeductRule(const SProrateDeductRuleDtlMap& ProrateDeductRuleDtl,const int32 &ruleClass, 
															SProrateRuleMap &prorateRuleMap) throw (const CExpX&);
	
	bool get_compositeDeductRule(
		const int32 offer_prod_id,
		const int32 bill_type,
		const int32 main_promotion,
		SCompositeDeductRule &sCompositeDeductRule) const;
private:
	SCompositeDeductRuleMap m_sCompositeDeductRule;		//销售品扣费规则
	
	SProrateRuleMap m_sFreeResProrateRuleMap;		//免费资源的折半规则
	SProrateRuleMap m_sAccumulateProrateRuleMap;		//累计量的折半规则
};

END_NAMESPACE

#endif
