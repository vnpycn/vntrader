#include  <iostream> 
#include "windows.h"
#include "CSetupComDi.h"
using   namespace   std; 

void main()
{

		HongLib::CSetupComDi adp;
	adp.QueryFromDatabase(TEXT("4D36E972-E325-11CE-BFC1-08002bE10318"));//////////������GUIDֵ������WINDOWSϵͳ������һ����
	for(PBYTE pb = adp.EnumSetupDi();pb != NULL;pb = adp.EnumSetupDi())
	{
		if(strcmp((const char*)pb,(const char*)TEXT("Realtek RTL8139/810x Family Fast Ethernet NIC")) == 0)///////�����������֣�ָ���Լ���Ҫ�������������ɣ�
		{
			cout<<"����ָ������"<<endl;
			if(adp.IsDisable())
			{
				cout<<"��ǰΪ����״̬ "<<endl;
			}
			else
			{
				cout<<"��ǰΪ����״̬ "<<endl;
			}
			cout<<"���ڽ�������..."<<endl;

			if(adp.DisableDev())
			{
				cout<<"���óɹ�"<<endl;
			}
			else
			{
				cout<<"����ʧ��"<<endl;
				return;
			}
if(adp.IsDisable())
			{
				cout<<"��ǰΪ����״̬ "<<endl;
			}
			else
			{
				cout<<"��ǰΪ����״̬ "<<endl;
			}
			cout<<"5�����������"<<endl;
			::Sleep(5000);
			if(adp.EnableDev())
			{
				cout<<"���óɹ�"<<endl;
			}
			else
			{
				cout<<"����ʧ��"<<endl;
			}
			if(adp.IsDisable())
			{
				cout<<"��ǰΪ����״̬ "<<endl;
			}
			else
			{
				cout<<"��ǰΪ����״̬ "<<endl;
			}
		}
	}
}
