/* $Id: ProrateDeductRule.cpp 311559 2013-04-17 07:54:28Z qijf $  */
#include "ProrateDeductRule.h"
//#include <aiset.h>

BEGIN_NAMESPACE

inline uint64 MakeKey(const uint32 key1,const uint32 key2)
{
	uint64 newKey = key1;
	
	return newKey<<32|key2;
}
CProrateDeductRule::CProrateDeductRule()
{
}

CProrateDeductRule::~CProrateDeductRule()
{
}

//初始化接口
void CProrateDeductRule::init(const xc::CSnapshot& cSnapShot)throw (const CExpX&)
{
	init_compositeDeductRule(cSnapShot);
	const SProrateDeductRuleDtlMap ProrateDeductRuleDtl = init_prorateDeductRule(cSnapShot);
	
	try{
		//免费资源
		check_prorateDeductRule(ProrateDeductRuleDtl,1,m_sFreeResProrateRuleMap);	
		//累积相关
		check_prorateDeductRule(ProrateDeductRuleDtl,2,m_sAccumulateProrateRuleMap);

	}
	catch(const CExpX& e){
		RATING_TRACES("error: %s", e.what());
		throw;
	}
}

void CProrateDeductRule::check_prorateDeductRule(const SProrateDeductRuleDtlMap& ProrateDeductRuleDtl,
															const int32 &ruleClass,SProrateRuleMap &prorateRuleMap) throw (const CExpX&)
{
	for (SProrateDeductRuleDtlMap::const_iterator itr = ProrateDeductRuleDtl.begin(); itr != ProrateDeductRuleDtl.end(); ++itr)
	{//对于每个折半规则IDprorate_deduct_rule_id
		
		const aivector<SProrateDeductRuleDtl>& vecProrateDeductRule = itr->second;
		ASSERT(!vecProrateDeductRule.empty());
		
		SProrateRule prorateRule;
		for (size_t i = 0; i < vecProrateDeductRule.size(); ++i)
		{
			const SProrateDeductRuleDtl& prorateDeductRule = vecProrateDeductRule[i];
			if (prorateDeductRule.rule_class != 0 && prorateDeductRule.rule_class != ruleClass)
			{
				continue;
			}
			
			//首周期折半规则
			if(prorateDeductRule.effect_type==0)
			{
				prorateRule.prorate_type_first = prorateDeductRule.prorate_type;
			}
			
			//尾周期折半规则
			if(prorateDeductRule.effect_type==1)
			{
				prorateRule.prorate_type_last = prorateDeductRule.prorate_type;
			}
		}
		prorateRuleMap[itr->first] = prorateRule;
	}
}

//销售品扣费规则
void CProrateDeductRule::init_compositeDeductRule(const xc::CSnapshot& cSnapShot)
{
	//sql: SELECT PRODUCT_OFFERING_ID,PRORATE_DEDUCT_RULE_ID FROM PM_COMPOSITE_DEDUCT_RULE
	xc::list<CPmCompositeDeductRule>::iterator itr_end;
	xc::list<CPmCompositeDeductRule>::iterator itr;

	const int32 iRet = odac_app_lookup_CPmCompositeDeductRule(cSnapShot, itr, itr_end);
	if (iRet < 0)
	{
		RATING_INFO("odac_app_lookup_CPmCompositeDeductRule return no data.");
	}
	else
	{
		for (; itr != itr_end; ++itr) 
		{	
			
			SCompositeDeductRule compositeDeductRule;
			compositeDeductRule.offer_prod_id = itr->get_productOfferingId();
			compositeDeductRule.prorate_deduct_rule_id = itr->get_prorateDeductRuleId();
			compositeDeductRule.deduct_flag = itr->get_deductFlag();
			compositeDeductRule.resource_flag = itr->get_resourceFlag();//add by gezb 2012-11-26
			if( 1 != compositeDeductRule.resource_flag )
			{
				compositeDeductRule.resource_flag = 0;//默认取0
			}
			compositeDeductRule.main_promotion = itr->get_mainPromotion();

			const int64 key = MakeKey(compositeDeductRule.offer_prod_id,itr->get_billingType());
			aimap<int64,SCompositeDeductRuleList>::iterator itr_find = m_sCompositeDeductRule.find(key);
			if(itr_find != m_sCompositeDeductRule.end())
			{
				itr_find->second.push_back(compositeDeductRule);
			}
			else
			{
				SCompositeDeductRuleList listCompositeDeductRule;
				listCompositeDeductRule.push_back(compositeDeductRule);
				m_sCompositeDeductRule.insert(SCompositeDeductRuleMap::value_type(key,listCompositeDeductRule));
			}

		}	
	}
}

//半月扣费折半规则明细
const SProrateDeductRuleDtlMap CProrateDeductRule::init_prorateDeductRule(const xc::CSnapshot& cSnapShot)
{
	//sql: SELECT A.PRORATE_DEDUCT_RULE_ID, A.RULE_CLASS,EFFECT_TYPE, NVL(A.PRORATE_TYPE,0) FROM PM_PRORATE_DEDUCT_RULE_DTL A, PM_PRORATE_DEDUCT_RULE B WHERE A.PRORATE_DEDUCT_RULE_ID = B.PRORATE_DEDUCT_RULE_ID AND A.RULE_CLASS IN (0,1,2)
	
	SProrateDeductRuleDtlMap ProrateDeductRuleDtl;
	
	xc::list<CPmProrateDeductRuleDtl>::iterator itr_end;
	xc::list<CPmProrateDeductRuleDtl>::iterator itr;
	
	const int32 iRet = odac_app_lookup_CPmProrateDeductRuleDtl(cSnapShot, itr, itr_end);
	if (iRet < 0)
	{
		RATING_INFO("odac_app_lookup_CPmProrateDeductRuleDtl return no data.");
	}
	else
	{	
		SProrateDeductRuleDtl prorateDeductRuleDtl;
		
		for (; itr != itr_end; ++itr) 
		{	
			if( itr->get_effectType()!=0 && itr->get_effectType()!=1 )
			{
				continue;//yuzj3 2012 08 09, 批价只要EffectType为0,1的记录，为0表示首周期折扣，为1表示尾周期折扣
			}
			prorateDeductRuleDtl.prorate_deduct_rule_id = itr->get_prorateDeductRuleId();		
			prorateDeductRuleDtl.rule_class = itr->get_ruleClass();					
			prorateDeductRuleDtl.effect_type = itr->get_effectType();					
			prorateDeductRuleDtl.prorate_type = itr->get_prorateType();	
			
			SProrateDeductRuleDtlMap::iterator itr_temp = ProrateDeductRuleDtl.find(prorateDeductRuleDtl.prorate_deduct_rule_id);
			if (itr_temp != ProrateDeductRuleDtl.end())
			{
				itr_temp->second.push_back(prorateDeductRuleDtl);
			}
			else
			{
				aivector<SProrateDeductRuleDtl> vecProrateDeductRule;
				
				vecProrateDeductRule.push_back(prorateDeductRuleDtl);
				ProrateDeductRuleDtl[prorateDeductRuleDtl.prorate_deduct_rule_id] = vecProrateDeductRule;
			}
		}
	}
	
	return ProrateDeductRuleDtl;
}

bool CProrateDeductRule::get_compositeDeductRule(
		const int32 offer_prod_id,
		const int32 bill_type,
		const int32 main_promotion,
		SCompositeDeductRule &sCompositeDeductRule) const
{
	bool bRet = false;
	SCompositeDeductRuleMap::const_iterator itr_find = m_sCompositeDeductRule.find(MakeKey(offer_prod_id,bill_type));
	if(itr_find != m_sCompositeDeductRule.end())
	{
		
		for(size_t i = 0; i < itr_find->second.size(); ++i)
		{
			//RATING_TRACES("main_promotion1[%d] main_promotion2[%d] ",itr_find->second.at(i).main_promotion,main_promotion);
			if(itr_find->second.at(i).main_promotion == -1
				||itr_find->second.at(i).main_promotion == main_promotion)
			{
				sCompositeDeductRule = itr_find->second.at(i);
				bRet = true;
				break;
			}
		}		
	}
	
	//RATING_TRACES("bRet[%d]",bRet);
	return bRet;

}

//获取属于免费资源的半月扣费折半规则中nProrateType和effectType
void CProrateDeductRule::get_freeResProrateRule(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion, SProrateRule& prorate_rule) const
{
	SCompositeDeductRule sCompositeDeductRule;
	//先获取sCompositeDeductRule，里面包含prorate_deduct_rule_id
	bool bGet = get_compositeDeductRule(offer_prod_id,bill_type,main_promotion,sCompositeDeductRule);
	bool bFind = false;
	if (bGet)
	{
		//根据prorate_deduct_rule_id去查对应的折半规则明细
		SProrateRuleMap::const_iterator prorate_itr = m_sFreeResProrateRuleMap.find(sCompositeDeductRule.prorate_deduct_rule_id);
		if (prorate_itr != m_sFreeResProrateRuleMap.end())
		{
			prorate_rule = prorate_itr->second;
			//effectType = prorate_itr->second.effectType;
			//prorate_type = prorate_itr->second.prorate_type;
			bFind = true;
		}
	}
	
	if(!bFind)
	{
		//没有找到折半规则，则认为是无折扣。
		prorate_rule.prorate_type_first = 0;
		prorate_rule.prorate_type_last = 0;
	}
}

//获取属于免费资源的半月扣费折半规则中nProrateType和effectType
void CProrateDeductRule::get_accumulateProrateRule(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion, SProrateRule& prorate_rule) const
{
	SCompositeDeductRule sCompositeDeductRule;
	bool bGet = get_compositeDeductRule(offer_prod_id,bill_type,main_promotion,sCompositeDeductRule);
	bool bFind = false;
	if (bGet)
	{
		RATING_TRACES("offer_prod_id[%d] prorate_deduct_rule_id[%d] ",offer_prod_id,sCompositeDeductRule.prorate_deduct_rule_id);
		SProrateRuleMap::const_iterator prorate_itr = m_sAccumulateProrateRuleMap.find(sCompositeDeductRule.prorate_deduct_rule_id);
		if (prorate_itr != m_sAccumulateProrateRuleMap.end())
		{
			prorate_rule = prorate_itr->second;
			//effectType = prorate_itr->second.effectType;
			//prorate_type = prorate_itr->second.prorate_type;
			bFind = true;
		}
	}
	
	if(!bFind)
	{
		//没有找到折半规则，则认为是无折扣。
		prorate_rule.prorate_type_first = 0;
		prorate_rule.prorate_type_last = 0;
	}
}

int32 CProrateDeductRule::get_deductFlag(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion)const throw (const CExpX&)
{
	SCompositeDeductRule sCompositeDeductRule;
	bool bGet = get_compositeDeductRule(offer_prod_id,bill_type,main_promotion,sCompositeDeductRule);
	if(!bGet)
	{
		throw CExpX(622039)<< offer_prod_id<< bill_type<< main_promotion;
		//_ERROE_MSG   The PRODUCT_OFFER_ID[?] with BILL_TYPE[?],MAIN_PROMOTION[?] is not in PM_COMPOSITE_DEDUCT_RULE, suggest: check deduct rule config in table PM_COMPOSITE_DEDUCT_RULE
	}
	return sCompositeDeductRule.deduct_flag;
}

int32 CProrateDeductRule::get_resourceFlag(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion)const throw (const CExpX&)
{
	SCompositeDeductRule sCompositeDeductRule;
	bool bGet = get_compositeDeductRule(offer_prod_id,bill_type,main_promotion,sCompositeDeductRule);
	if(!bGet)
	{
		throw CExpX(622039) << offer_prod_id<< bill_type<< main_promotion;
		//_ERROE_MSG   The PRODUCT_OFFER_ID[?] with BILL_TYPE[?],MAIN_PROMOTION[?] is not in PM_COMPOSITE_DEDUCT_RULE, suggest: check deduct rule config in table PM_COMPOSITE_DEDUCT_RULE
	}
	return sCompositeDeductRule.resource_flag;
}

//重新加载数据用
void CProrateDeductRule::clear()
{
	m_sCompositeDeductRule.clear();
	m_sFreeResProrateRuleMap.clear();
	m_sAccumulateProrateRuleMap.clear();
}


END_NAMESPACE
