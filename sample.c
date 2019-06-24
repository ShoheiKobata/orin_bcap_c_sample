#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "include/bCAPClient/bcap_client.h"
//#include "include/bcap_common.h"
/*
#include "include/bcap_funcid.h"
#include "include/dn_additional.h"
#include "include/dn_com.h"
#include "include/dn_common.h"
#include "include/dn_device.h"
#include "include/dn_robotalk.h"
#include "include/dn_socket.h"
#include "include/dn_tcp.h"
#include "include/dn_thread.h"
#include "include/dn_udp.h"
*/

#define TARGET_RC8_IP "192.168.0.1"

int main(void){

    int fd;
    uint32_t hCtrl;
    BSTR bstr1, bstr2, bstr3, bstr4;
    HRESULT hr;

    printf("Open connection to tcp:%S\n",TARGET_RC8_IP);
    hr = bCap_Open_Client("tcp:" TARGET_RC8_IP,1000,3,&fd);

    if(SUCCEEDED(hr)){
		/* Send SERVICE_START packet */
		bCap_ServiceStart(fd, NULL);

		bstr1 = SysAllocString(L"");                    /* Name */
		bstr2 = SysAllocString(L"CaoProv.DENSO.VRC");   /* Provider */
		bstr3 = SysAllocString(L"localhost");			/* Machine */
		bstr4 = SysAllocString(L"");					/* Option */

		/* Connect RC8 */
		hr = bCap_ControllerConnect(fd, bstr1, bstr2, bstr3, bstr4, &hCtrl);

		SysFreeString(bstr1);
		SysFreeString(bstr2);
		SysFreeString(bstr3);
		SysFreeString(bstr4);

		/* Check the control mode switch. It should be AUTO. */
		if(SUCCEEDED(hr)){
			uint32_t hMode;

			bstr1 = SysAllocString(L"@Mode");	// Name
			bstr2 = SysAllocString(L"");		// Option

			/* Get mode switch variable handle */
			hr = bCap_ControllerGetVariable(fd, hCtrl, bstr1, bstr2, &hMode);

			SysFreeString(bstr1);
			SysFreeString(bstr2);

			if(SUCCEEDED(hr)){
				VARIANT vMode;

				VariantInit(&vMode);

				bCap_VariableGetValue(fd, hMode, &vMode);
				VariantChangeType(&vMode, &vMode, 0, VT_I4);
				// vMode.vt == VT_I4
				if (vMode.lVal != 3) {
					printf("Change RC8-MODE to AUTO !!\n");
					hr = E_FAIL;
				}

				/* Release variable handle of @Mode */
				bCap_VariableRelease(fd, &hMode);
			}
		}
    }
    return 0;
}