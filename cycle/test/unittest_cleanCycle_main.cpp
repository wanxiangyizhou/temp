#include "unittest_cleanCycle_main.h"
#include <errorCodeManage.h>

const char* g_pDumpIn = NULL;//����Զ������ĳ�����������
const char* g_pDumpOut = NULL;//����Զ������ĳ�������ĳ���
//��ʽ1:ֱ�ӵ���tm.runAll(-1, false)����������������
//��ʽ2:�ȵ���tm.addSuite��tm.addTest���Ҫ���Ե��������ٵ���tm.runAll()��������ӵ�����
int main(int argc, char **argv)
{
	int ret = 0;
	TestSuiteManager<XmlTestReporter> tm(0);
	if (tm.Init(argc,argv) != 0) return -1;
	/**code start**/
	
	//������ע��
	rating_charging::CErrorCodeManage::registerErrorCode();

	tm.runAll(-1, false);//Ĭ���������е�����

	/**code end**/
	if (tm.saveReport() != 0) return -1;
	return ret;
}

//cpp2unit_version_date:20110906
