/* $Id: IProrateDeductRule.h 314670 2013-04-24 08:32:30Z caiwei $  */
#ifndef _IPRORATE_DEDUCT_RULE_H_
#define _IPRORATE_DEDUCT_RULE_H_

#include <charging_exp_x.h>
#include <xdr_def_sdl_c.h>

BEGIN_NAMESPACE
//�����Դ���۰����
struct SProrateRule
{
public:
	SProrateRule(): prorate_type_first(0), prorate_type_last(0)
	{
	}
	SProrateRule(const SProrateRule& rh): prorate_type_first(rh.prorate_type_first), prorate_type_last(rh.prorate_type_last)
	{
	}
	SProrateRule& operator= (const SProrateRule& rh)
	{
		if (&rh == this){return (*this);}
		
		prorate_type_first = rh.prorate_type_first;
		prorate_type_last = rh.prorate_type_last;
		return (*this);
	}
public:
	int32 prorate_type_first; //�������۰����
	int32 prorate_type_last; //β�����۰����
};

class IProrateDeductRule
{
public:
	IProrateDeductRule(){};
	virtual ~IProrateDeductRule(){};
public:
	//��ȡ���������Դ�İ��¿۷��۰������nProrateType��effectType
	virtual void get_freeResProrateRule(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion, SProrateRule& prorate_rule)const = 0;

	//��ȡ�����ۻ���صİ��¿۷��۰������nProrateType��effectType
	virtual void get_accumulateProrateRule(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion, SProrateRule& prorate_rule)const = 0;
	
	//��ȡ������ÿ۷ѷ�ʽ
	virtual int32 get_deductFlag(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion)const throw (const CExpX&) =0;

	//�����Դ�����Ƿ������̷ѿۼ�����
	virtual int32 get_resourceFlag(const int32 offer_prod_id,const int32 bill_type,const int32 main_promotion)const throw (const CExpX&) =0;

public:
	static const int32 get_mainProdOfferId(const MXdr::SRateProdIdMap &rateProdMap) 
	{
		int32 llMainProdOfferId = -1;
		for(MXdr::SRateProdIdMap::const_iterator itr = rateProdMap.begin();
			itr != rateProdMap.end(); ++itr)
		{
			if(itr->second.get_prodType() == 1)
			{
				llMainProdOfferId = itr->second.get_offerProdId();
				break;
			}
		}
		
		return llMainProdOfferId;
	}

private:
	IProrateDeductRule(const IProrateDeductRule&);
	IProrateDeductRule& operator=(const IProrateDeductRule&);
};

END_NAMESPACE

#endif 
