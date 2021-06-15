#include  <iostream> 
#include "windows.h"
#include "CSetupComDi.h"
using   namespace   std; 

void main()
{

		HongLib::CSetupComDi adp;
	adp.QueryFromDatabase(TEXT("4D36E972-E325-11CE-BFC1-08002bE10318"));//////////网卡的GUID值，所有WINDOWS系统上面是一样的
	for(PBYTE pb = adp.EnumSetupDi();pb != NULL;pb = adp.EnumSetupDi())
	{
		if(strcmp((const char*)pb,(const char*)TEXT("Realtek RTL8139/810x Family Fast Ethernet NIC")) == 0)///////我网卡的名字（指定自己需要操作的网卡即可）
		{
			cout<<"发现指定网卡"<<endl;
			if(adp.IsDisable())
			{
				cout<<"当前为禁用状态 "<<endl;
			}
			else
			{
				cout<<"当前为启用状态 "<<endl;
			}
			cout<<"正在禁用网卡..."<<endl;

			if(adp.DisableDev())
			{
				cout<<"禁用成功"<<endl;
			}
			else
			{
				cout<<"禁用失败"<<endl;
				return;
			}
if(adp.IsDisable())
			{
				cout<<"当前为禁用状态 "<<endl;
			}
			else
			{
				cout<<"当前为启用状态 "<<endl;
			}
			cout<<"5秒后启用网卡"<<endl;
			::Sleep(5000);
			if(adp.EnableDev())
			{
				cout<<"启用成功"<<endl;
			}
			else
			{
				cout<<"启用失败"<<endl;
			}
			if(adp.IsDisable())
			{
				cout<<"当前为禁用状态 "<<endl;
			}
			else
			{
				cout<<"当前为启用状态 "<<endl;
			}
		}
	}
}
