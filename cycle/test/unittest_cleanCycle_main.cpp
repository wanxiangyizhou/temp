#include "unittest_cleanCycle_main.h"
#include <errorCodeManage.h>

const char* g_pDumpIn = NULL;//框架自动导入的某个用例的入参
const char* g_pDumpOut = NULL;//框架自动导入的某个用例的出参
//方式1:直接调用tm.runAll(-1, false)，运行所有用例；
//方式2:先调用tm.addSuite或tm.addTest添加要测试的用例，再调用tm.runAll()运行所添加的用例
int main(int argc, char **argv)
{
	int ret = 0;
	TestSuiteManager<XmlTestReporter> tm(0);
	if (tm.Init(argc,argv) != 0) return -1;
	/**code start**/
	
	//错误码注册
	rating_charging::CErrorCodeManage::registerErrorCode();

	tm.runAll(-1, false);//默认运行所有的用例

	/**code end**/
	if (tm.saveReport() != 0) return -1;
	return ret;
}

//cpp2unit_version_date:20110906
