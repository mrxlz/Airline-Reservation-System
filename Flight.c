// Flight.c : ����Ӧ�ó������ڵ㡣
//
#include "stdafx.h"


//����
typedef struct OrderForm{
	TCHAR IdNum[32];									//�����û����֤��
	int Order_Number;									//������
	int Tickets_Num;									//��Ʊ����
	TCHAR Flight_Number[16];							//�����
	TCHAR Departure[16];								//������
	TCHAR Destination[16];								//Ŀ�ĵ�
	TCHAR Date[16];										//��������
	TCHAR TakeOff_Time[16];								//���ʱ��
	TCHAR Landing_Time[16];								//����ʱ��
	struct OrderForm *Next;								//���ж�������next
	struct OrderForm *psgNext;							//�û���������next
}OrderForm;

//�˿Ͷ�������
typedef struct PsgOrderLink{
	int OrderNum;										//������Ŀ
	OrderForm *head;									//ͷ���
	OrderForm *tail;									//β���
}PsgOrderLink;

//���ж�������
typedef struct AllOrderLink{
	int AllOrderNum;									//���ж�����Ŀ
	OrderForm *head;									//ͷ���
	OrderForm *tail;									//β���
}AllOrderLink;

//�˿�
typedef struct Passenger{
	TCHAR Name[16];								        //����
	TCHAR IdNum[32];									//���֤����
	TCHAR PassWord[32];							    	//����
	int TicketNum;										//������Ŀ
	PsgOrderLink OrderLink;								//�û����ж���
	struct Passenger *Next;
}Passenger;

//�˿�����
typedef struct PsgLinkList{
	int PsgNum;											//�˻�����
	Passenger *head;									//ͷ���
	Passenger *tail;									//β���
}PsgLinkList;

//����
typedef struct Flight{
	double Fare;										//Ʊ��
	int Seat_Number;									//��λ��
	int Vacant_Seat;									//������λ��
	TCHAR Discount[16];									//�ۿ�
	TCHAR Flight_Number[16];							//�����
	TCHAR Departure[16];								//������
	TCHAR Destination[16];								//Ŀ�ĵ�
	TCHAR Date[16];										//��������
	TCHAR TakeOff_Time[16];								//���ʱ��
	TCHAR Landing_Time[16];								//����ʱ��
	struct Flight *Next;
}Flight;

//��������
typedef struct FlightLinkList{
	Flight *head;										//ͷ���
	Flight *tail;										//β���
	int Flight_Number;									//������Ŀ
}FilghtLinkList;


// ȫ�ֱ���: 
HICON hIcon;
HINSTANCE hInst;										//��ǰʵ��
static TCHAR szBuffer[256];								//������
static PsgLinkList psglink;								//�����˻�_����
static Passenger *passenger;							//��½�˻���Ϣ
static AllOrderLink allorder;							//���ж���_����
static FilghtLinkList flightlink;						//���к���_����



//��������
BOOL AccountLogIn(HWND);										//�˻���½
BOOL AccountRegister(HWND);										//ע���˻�
BOOL ReadFlightData(HWND);										//���뺽����Ϣ
BOOL ReadAccountData(HWND);										//�����˻�����
BOOL ReadAccountOrder(HWND, Passenger*);						//�������ж������˻�����
BOOL SearchFlight(HWND);										//��ѯ����
BOOL BookTickets(HWND);											//��Ʊ
BOOL _Book_Tickets(HWND, Flight*, int);							//��Ʊ
BOOL Recommend(HWND, Flight*, int);								//���ཨ��
BOOL ReturnTickets(HWND);										//��Ʊ
BOOL EntryFlight(HWND);											//¼�뺽��
BOOL ModifyFlight(HWND);										//�޸ĺ�����Ϣ
BOOL PrintFlight(HWND, Flight*);								//���������Ϣ
BOOL WriteFlightData(HWND);										//���溽����Ϣ
BOOL WriteAccountData(HWND);									//�����˻�����
BOOL WriteOrderData(HWND);										//���涩����Ϣ
BOOL CALLBACK LogInDlgProc(HWND, UINT, WPARAM, LPARAM);			//��½���ڴ��ڹ���
BOOL CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);			//�����洰�ڹ���
BOOL CALLBACK NameDlgProc(HWND, UINT, WPARAM, LPARAM);			//��ȡ��ע���û��������ڹ���
BOOL CALLBACK FlightNumDlgProc(HWND, UINT, WPARAM, LPARAM);		//��ȡ�û������Ʊ�������ڹ���
BOOL CALLBACK EntryFlightProc(HWND, UINT, WPARAM, LPARAM);		//¼�뺽�ര�ڹ���
BOOL CALLBACK ModifyFlightProc(HWND, UINT, WPARAM, LPARAM);		//�޸ĺ�����Ϣ���ڹ���



//������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{

	hInst = hInstance;
	InitCommonControls();
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_LOGINDLG), NULL, (DLGPROC)LogInDlgProc);
}//WinMain


//��½����_���ڹ���
BOOL CALLBACK LogInDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//WM_INITDIALOG�ǵ���Ի�����ӿؼ�ȫ��������ϣ���Ҫ��ʾ���ݵ�ʱ���͵���Ϣ
		//��˿�����WM_INITDIALOG��Ϣ��Ӧ��������ӶԱ༭��ؼ��ĳ�ʼ�����޸�
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		ReadAccountData(hwndDlg);					//��ȡ�˻�����������
		ReadFlightData(hwndDlg);					
	}//WM_INITDIALOG
		return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_LOGIN:
			AccountLogIn(hwndDlg);					//��½
			break;
		case IDC_REGISTER:
			AccountRegister(hwndDlg);				//ע��
			break;
		}//switch

	}//WM_COMMAND
		return TRUE;

	}//switch
	return FALSE;
}//LogInDlgProc����


//������_���ڹ���
BOOL CALLBACK MainDlgProc(HWND hMainDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hMainDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}//WM_INITDIALOG
		return TRUE;

	case WM_CLOSE:
	{
		//�رճ���ʱ����Ϣ����												
		if (WriteOrderData(hMainDlg) && WriteAccountData(hMainDlg) && WriteFlightData(hMainDlg))
			EndDialog(hMainDlg, 0);
		else
			MessageBox(hMainDlg, TEXT("������Ϣ����"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_SEARCH:
		{
			SearchFlight(hMainDlg);						//���Һ���	
			break;
		}
		case IDC_TICKET:
		{
			BookTickets(hMainDlg);						//��Ʊ
			break;
		}
		case IDC_RETURNTICKET:
		{
			ReturnTickets(hMainDlg);					//��Ʊ
			break;
		}
		case IDC_ENTRY:
		{												//¼�뺽��Ի���
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ENTRY), NULL, (DLGPROC)EntryFlightProc);
			break;
		}
		case IDC_MODIFYFLIGHT:
		{												//�޸ĺ�����Ϣ�Ի���
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MODIFYFLT), NULL, (DLGPROC)ModifyFlightProc);
			break;
		}
		}//switch
	}//WM_COMMAND
		return TRUE;
	}//switch
	return FALSE;
}//MainDlgProc()


//��ȡ�û������Ʊ����_���ڹ���
BOOL CALLBACK FlightNumDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{

	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}//WM_INITDIALOG
		return TRUE;

	case WM_CLOSE:
	{
		wsprintf(szBuffer, TEXT("\0"));
		EndDialog(hwndDlg, FALSE);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			wsprintf(szBuffer, TEXT("\0"));
			GetDlgItemText(hwndDlg, IDC_TICKETNUM, szBuffer, 256);
			EndDialog(hwndDlg, TRUE);
			break;
		}//switch
	}//WM_COMMAND
		return TRUE;

	}//switch
	return FALSE;
}


//��ȡ��ע���û�����_���ڹ���
BOOL CALLBACK NameDlgProc(HWND hNameDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hNameDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
		return TRUE;

	case WM_CLOSE:
	{
		wsprintf(passenger->Name, TEXT("\0"));
		EndDialog(hNameDlg, 0);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_GETNAMEOK:
		{
			GetDlgItemText(hNameDlg, IDC_GETNAME, passenger->Name, 16);
			EndDialog(hNameDlg, TRUE);
			break;
		}
		case IDC_GETNAMECANCEL:
		{
			wsprintf(passenger->Name, TEXT("\0"));
			EndDialog(hNameDlg, TRUE);
			break;
		}

		}//stitch

	}//WM_COMMAND
	return TRUE;

	}//stitch

	return FALSE;

}//NameDlgProc()


//¼�뺽��_���ڹ���
BOOL CALLBACK EntryFlightProc(HWND hEntryDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hEntryDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
	return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hEntryDlg, 0);
	}
	return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_ADDFLT:
		{
			EntryFlight(hEntryDlg);						//¼�뺽��
			break;
		}
		case IDC_QUIT:
		{
			EndDialog(hEntryDlg, FALSE);
			break;
		}

		}//stitch

	}//WM_COMMAND
		return TRUE;

	}//stitch

	return FALSE;
}


//�޸ĺ�����Ϣ_���ڹ���
BOOL CALLBACK ModifyFlightProc(HWND hModifyDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hModifyDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
		return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hModifyDlg, 0);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_MODIFY:
		{
			ModifyFlight(hModifyDlg);					//�޸ĺ�����Ϣ
			break;
		}
		case IDC_QUIT:
		{
			EndDialog(hModifyDlg, FALSE);
			break;
		}

		}//stitch

	}//WM_COMMAND
		return TRUE;
	}//stitch

	return FALSE;
}//ModifyFlightProc()


//�����˻���Ϣ
BOOL ReadAccountData(HWND hwndDlg){

	FILE *fp;

	passenger = (Passenger *)malloc(sizeof(Passenger));										//Ϊ��¼�˻������ڴ�
	if (!passenger){
		MessageBox(hwndDlg, TEXT("�ڴ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
		EndDialog(hwndDlg, 0);
		return FALSE;
	}//if

	psglink.head = NULL;
	psglink.tail = NULL;
	psglink.PsgNum = 0;

	if ((fp = fopen(".\\AccountData.txt", "r+")) == NULL){		//���ļ�
		MessageBox(hwndDlg, TEXT("�˻��ļ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
		EndDialog(hwndDlg, 0);
	}//if		

	while (!feof(fp)){
		Passenger *p = (Passenger *)malloc(sizeof(Passenger));
		if (!p){
			MessageBox(hwndDlg, TEXT("�ڴ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
			EndDialog(hwndDlg, 0);
			return FALSE;
		}//if

		//�����˻����������֤�š�����
		if (fscanf(fp, "%s%s%s", p->Name, p->IdNum, p->PassWord) == EOF){
			free(p); break;
		}

		p->Next = NULL;
		if (psglink.head == NULL)						//�����һ���˻���Ϣʱ��ͷ��β����ָ��p
			psglink.head = p;
		else
			psglink.tail->Next = p;						//����β���Nextָ��p
		psglink.tail = p;								//β���ָ��p
		psglink.PsgNum++;								//��ע���˻�����

	}//while
	fclose(fp);											//�ر��ļ�
	return TRUE;
}//ReadAccountData(HWND)


//���뺽����Ϣ
BOOL ReadFlightData(HWND hwndDlg){

	int flag = 0;
	FILE *fp;

	flightlink.Flight_Number = 0;
	flightlink.head = NULL;
	flightlink.tail = NULL;

	if ((fp = fopen(".\\FlightData.txt", "r")) == NULL){		//���ļ�
		MessageBox(hwndDlg, TEXT("�����ļ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
		EndDialog(hwndDlg, 0);
	}//if

	while (!feof(fp)){
		Flight *p = (Flight *)malloc(sizeof(Flight));
		if (!p){
			MessageBox(hwndDlg, TEXT("�ڴ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
			EndDialog(hwndDlg, 0);
			return FALSE;
		}//if
		//���뺽����Ϣ
		if (fscanf(fp, "%s%lf%s%d%d%s%s%s%s%s",
			p->Flight_Number,
			&p->Fare,
			p->Discount,
			&p->Seat_Number,
			&p->Vacant_Seat,
			p->Departure,
			p->Destination,
			p->Date,
			p->TakeOff_Time,
			p->Landing_Time) == EOF)
		{
			free(p);
			break;
		}
		p->Next = NULL;
		if (flightlink.head == NULL)					//���������
			flightlink.head = p;
		else
			flightlink.tail->Next = p;
		flightlink.tail = p;
		flightlink.Flight_Number++;

	}//while
	fclose(fp);											//�ر��ļ�
	return TRUE;
}//ReadFlightData()


//��ȡ������Ϣ
BOOL ReadAccountOrder(HWND hwndDlg, Passenger *passenger){

	FILE *fp;
	allorder.AllOrderNum = 0;							//���������ʼ��
	allorder.head = NULL;
	allorder.tail = NULL;
	passenger->TicketNum = 0;							//�˿Ͷ��������ʼ��
	passenger->OrderLink.head = NULL;
	passenger->OrderLink.tail = NULL;
	passenger->OrderLink.OrderNum = 0;

	if ((fp = fopen(".\\OrderForm.txt", "r")) == NULL){	//���ļ�
		MessageBox(hwndDlg, TEXT("�����ļ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	while (!feof(fp)){
		OrderForm *p = (OrderForm *)malloc(sizeof(OrderForm));
		if (!p){
			MessageBox(hwndDlg, TEXT("�ڴ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
			EndDialog(hwndDlg, 0);
			return FALSE;
		}//if
		if (fscanf(fp, "%s%d%d%s%s%s%s%s%s",			//��ȡ������Ϣ
			p->IdNum,									
			&p->Order_Number,
			&p->Tickets_Num,
			p->Flight_Number,
			p->Departure,
			p->Destination,
			p->Date,
			p->TakeOff_Time,
			p->Landing_Time) == EOF)
		{
			free(p);
			break;
		}
		p->Next = NULL;
		p->psgNext = NULL;

		if (allorder.head == NULL){						//�����������������ĩβ
			allorder.head = p;
		}
		else
			allorder.tail->Next = p;
		allorder.tail = p;
		allorder.AllOrderNum++;							//������Ŀ����

		if (!lstrcmp(p->IdNum, passenger->IdNum)){		//���ö���Ϊ��ǰ��¼�˻��������������û���������ĩβ
			if (passenger->OrderLink.head == NULL){		//����������û���������ĩβ
				passenger->OrderLink.head = p;
			}
			else
				passenger->OrderLink.tail->psgNext = p;
			passenger->OrderLink.tail = p;
			passenger->TicketNum += p->Tickets_Num;
			passenger->OrderLink.OrderNum++;			//������Ŀ����
		}//if
	}//while
	fclose(fp);
	return TRUE;
}//ReadAccountOrder()


//��½�û���֤
BOOL AccountLogIn(HWND hwndDlg){

	int flag = 0;																//flag��=0����û���ע�ᣬflag==0����û���δע��
	Passenger *p;

	GetDlgItemText(hwndDlg, IDC_IDEDIT, passenger->IdNum, 256);					//��ȡ�û�����ID������
	GetDlgItemText(hwndDlg, IDC_PSWEDIT, passenger->PassWord, 256);				

	p = psglink.head;
	while (p){																	//���û�������ҵ�½�û�ID
		if (!lstrcmp(passenger->IdNum, p->IdNum)){
			flag++;																//�û���ע��
			if (!lstrcmp(passenger->PassWord, p->PassWord)){					//����ƥ�䣬��½�ɹ�
				lstrcpy(passenger->Name, p->Name);
				MessageBox(hwndDlg, TEXT("��½�ɹ�������ȷ���������������"), TEXT("��½�ɹ�"), MB_OK | MB_ICONINFORMATION);
				break;
			}//if
			else{																//��������˳�ѭ��
				MessageBox(hwndDlg, TEXT("�����������������"), TEXT("�������"), MB_OK | MB_ICONERROR);
				return FALSE;
			}//else
		}//if
		p = p->Next;
	}//while
	if (!flag){																	//�û���δע��
		if (lstrlen(passenger->PassWord) == 0)
			MessageBox(hwndDlg, TEXT("���������룡"), TEXT("��Ϣ"), MB_OK | MB_ICONINFORMATION);
		else
			MessageBox(hwndDlg, TEXT("���˻���δע�ᣬ����ע��"), TEXT("��Ϣ"), MB_OK | MB_ICONINFORMATION);
	}//if
	else{																		//�رյ�½���棬����������
		EndDialog(hwndDlg, TRUE);
		if (ReadAccountOrder(hwndDlg, passenger))								//��ȡ�û�����
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINDLG), NULL, (DLGPROC)MainDlgProc);
	}//else

	return TRUE;
}//AccountLogIn(HWND)


//�û�ע��
BOOL AccountRegister(HWND hwndDlg){

	GetDlgItemText(hwndDlg, IDC_IDEDIT, passenger->IdNum, 256);			//��ȡ�û�����ID������
	GetDlgItemText(hwndDlg, IDC_PSWEDIT, passenger->PassWord, 256);		

	if (lstrlen(passenger->PassWord) == 0){
		MessageBox(hwndDlg, TEXT("������ע���û����룡"), TEXT("��Ϣ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	DialogBox(hInst, MAKEINTRESOURCE(IDD_NAMEDLG), NULL, (DLGPROC)NameDlgProc);	//��ȡ��ע���û�����

	if (lstrlen(passenger->Name) == 0)
		return FALSE;

	passenger->Next = NULL;

	if (psglink.head == NULL)										//����ע���˻�����������˻�����
		psglink.head = passenger;
	else
		psglink.tail->Next = passenger;								
	psglink.tail = passenger;										
	psglink.PsgNum++;												//�û���Ŀ����
	passenger->TicketNum = 0;
	passenger->OrderLink.head = NULL;								//�˿Ͷ��������ʼ��
	passenger->OrderLink.tail = NULL;
	passenger->OrderLink.OrderNum = 0;

	MessageBox(hwndDlg, TEXT("ע��ɹ����뵥��ȷ������������"), TEXT("ע��ɹ�"), MB_OK | MB_ICONINFORMATION);

	EndDialog(hwndDlg, TRUE);													//�رյ�½����

	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINDLG), NULL, (DLGPROC)MainDlgProc); //����������

	return TRUE;
}//AccountRegister(HWND)


//���Һ���
BOOL SearchFlight(HWND hMainDlg){
	int flag1 = 0;													//if(flag1==0) �����ء�Ŀ�ĵز�ѯ���࣬else������Ų麽��
	int flag2 = 0;
	Flight *p;
	TCHAR szBuffer[256] = { TEXT("\0") };
	TCHAR szDeparture[16], szDestination[16], szFltNum[16], szDate[16];

	HWND hEdit = GetDlgItem(hMainDlg, IDC_INFORMATION);

	GetDlgItemText(hMainDlg, IDC_DEPARTURE, szDeparture, 15);		//��ȡ�û����������
	GetDlgItemText(hMainDlg, IDC_DESTINATION, szDestination, 15);	//��ȡ�û�����Ŀ�ĵ�
	GetDlgItemText(hMainDlg, IDC_FLIGHTNUM, szFltNum, 15);			//��ȡ�û����뺽���
	GetDlgItemText(hMainDlg, IDC_DATE, szDate, 15);					//��ȡ�û�ѡ�񺽰�ʱ��

	SetDlgItemText(hMainDlg, IDC_INFORMATION, szBuffer);			//��պ�����Ϣ�����ı�


	if (lstrlen(szFltNum) || (!(lstrlen(szDeparture)) && !(lstrlen(szDestination))))
		flag1++;
	else if (!(lstrlen(szDeparture))){
		MessageBox(hMainDlg, TEXT("����������أ�"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	else if (!(lstrlen(szDestination))){
		MessageBox(hMainDlg, TEXT("������Ŀ�ĵأ�"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	if (flag1 && !(lstrlen(szFltNum))){
		MessageBox(hMainDlg, TEXT("����������ء�Ŀ�ĵػ򺽰���Բ�ѯ���࣡"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	p = flightlink.head;
	while (p){											//���Һ��ಢ���
		if (flag1){										//������Ų���
			if (!lstrcmp(p->Flight_Number, szFltNum) && !lstrcmp(p->Date, szDate)){
				if (!flag2){							//���ҵ����Ϻ��࣬���
					_stprintf(szBuffer, TEXT("�����\t������\tĿ�ĵ�\t���ʱ��\t����ʱ��\t����\t�۸�\t�ۿ�\t��������\n"));
					SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
					SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				}//if
				flag2++;
				PrintFlight(hMainDlg, p);				
			}//if
		}//if
		else{											//�������ء�Ŀ�ĵز���
			if (!lstrcmp(p->Departure, szDeparture) && !lstrcmp(p->Destination, szDestination) && !lstrcmp(p->Date, szDate)){
				if (!flag2){							//���ҵ����Ϻ��࣬���
					_stprintf(szBuffer, TEXT("�����\t������\tĿ�ĵ�\t���ʱ��\t����ʱ��\t����\t�۸�\t�ۿ�\t��������\n"));
					SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
					SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				}//if
				flag2++;
				PrintFlight(hMainDlg, p);				
			}//if
		}//else
		p = p->Next;
	}//while
	if (!flag2)
		MessageBox(hMainDlg, TEXT("�Բ���û�з�����Ҫ��ĺ���"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
	return TRUE;
}//SearchFlight()


//�����麽����Ϣ���
BOOL PrintFlight(HWND hwndDlg, Flight *p){
	TCHAR szBuffer[256];
	HWND hEdit = GetDlgItem(hwndDlg, IDC_INFORMATION);

	_stprintf(szBuffer, TEXT("\n%s\t%s\t%s\t%s\t%s\t%d\t%.2lf\t%s\t%s\n\n"),
		p->Flight_Number,				//�����
		p->Departure,					//������
		p->Destination,					//Ŀ�ĵ�
		p->TakeOff_Time,				//���ʱ��
		p->Landing_Time,				//����ʱ��
		p->Vacant_Seat,					//����
		p->Fare,						//�۸�
		p->Discount,					//�ۿ�
		p->Date);						//��������

	SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
	SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));

	return TRUE;
}


//��Ʊ
BOOL BookTickets(HWND hMainDlg){
	int tickets_num;
	Flight *p;
	TCHAR szFltNum[16], szDate[16];

	GetDlgItemText(hMainDlg, IDC_FLIGHTNUM, szFltNum, 15);			//��ȡ�û����뺽��š���������
	GetDlgItemText(hMainDlg, IDC_DATE, szDate, 15);					

	if (!lstrlen(szFltNum)){
		MessageBox(hMainDlg, TEXT("�����뺽����Զ�Ʊ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	DialogBox(hInst, MAKEINTRESOURCE(IDD_FLTNUM), NULL, (DLGPROC)FlightNumDlgProc);	//��ȡ��Ʊ��Ŀ

	if (!lstrlen(szBuffer))
		return FALSE;

	tickets_num = _ttoi(szBuffer);									//��Ʊ��Ŀ

	p = flightlink.head;
	while (p){														//���Һ��ಢ��Ʊ
		if (!lstrcmp(p->Flight_Number, szFltNum) && !lstrcmp(p->Date, szDate)){		//���ҵ����Ϻ���
			_Book_Tickets(hMainDlg, p, tickets_num);
			return TRUE;
		}
		p = p->Next;
	}//while
	MessageBox(hMainDlg, TEXT("�Բ��𣬸ú��಻���ڣ���ȷ����Ҫ��Ʊ�ĺ��ࡣ"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
	return FALSE;
}//BookTickets()     


//��Ʊ
BOOL _Book_Tickets(HWND hwndDlg, Flight *p, int Tickets_Num){

	OrderForm *new_order = (OrderForm *)malloc(sizeof(OrderForm));
	if (!new_order){
		MessageBox(hwndDlg, TEXT("�ڴ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (Tickets_Num > p->Vacant_Seat){					//��Ʊ���㣬�������麽��
		Recommend(hwndDlg, p, Tickets_Num);
		return FALSE;
	}

	new_order->Order_Number = ++allorder.AllOrderNum;	//�¶�����Ϣ���ܶ�����+1
	new_order->Tickets_Num = Tickets_Num;
	lstrcpy(new_order->IdNum, passenger->IdNum);
	lstrcpy(new_order->Flight_Number, p->Flight_Number);
	lstrcpy(new_order->Departure, p->Departure);
	lstrcpy(new_order->Destination, p->Destination);
	lstrcpy(new_order->Date, p->Date);
	lstrcpy(new_order->TakeOff_Time, p->TakeOff_Time);
	lstrcpy(new_order->Landing_Time, p->Landing_Time);
	new_order->Next = NULL;
	new_order->psgNext = NULL;

	if (allorder.head == NULL)							//�����������������ĩβ
		allorder.head = new_order;
	else
		allorder.tail->Next = new_order;
	allorder.tail = new_order;

	if (passenger->OrderLink.head == NULL)				//����������û���������ĩβ
		passenger->OrderLink.head = new_order;
	else
		passenger->OrderLink.tail->psgNext = new_order;
	passenger->OrderLink.tail = new_order;
	passenger->OrderLink.OrderNum++;
	passenger->TicketNum += Tickets_Num;

	p->Vacant_Seat -= Tickets_Num;						//������Ʊ����

	MessageBox(hwndDlg, TEXT("��Ʊ�ɹ���"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);

	return TRUE;
}//_Book_Tickets()


//�������麽��
BOOL Recommend(HWND hwndDlg, Flight *p, int Tickets_Num){

	SetDlgItemText(hwndDlg, IDC_INFORMATION, TEXT("\0"));	//��պ�����Ϣ��

	int flag = 0;
	TCHAR szBuffer[256] = { TEXT("\0") };
	HWND hEdit = GetDlgItem(hwndDlg, IDC_INFORMATION);
	Flight *q;

	q = flightlink.head;
	while (q){

		if (!(lstrcmp(p->Departure, q->Departure)) && !(lstrcmp(p->Destination, q->Destination)) &&
			lstrcmp(p->Flight_Number, q->Flight_Number) && Tickets_Num <= q->Vacant_Seat)
		{
			if (!flag){
				wsprintf(szBuffer, TEXT("������������Ʊ���㣬����Ϊ���麽��:\n"));
				SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				wsprintf(szBuffer, TEXT("\n"));
				SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				wsprintf(szBuffer, TEXT("�����\t������\tĿ�ĵ�\t���ʱ��\t����ʱ��\t����\t�۸�\t�ۿ�\t��������\n"));
				SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				++flag;
			}//if
			PrintFlight(hwndDlg, q);
		}//if
		q = q->Next;
	}//while

	if (!flag){
		wsprintf(szBuffer, TEXT("�Բ���������������Ʊ���㣬����Ҳ����û�з��������ĺ���...\n"));
		SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
		SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
	}
	return TRUE;
}//Recommend()


//��Ʊ
BOOL ReturnTickets(HWND hwndDlg){
	int flag = 0;
	int num = 0, tickets_num;
	Flight *flight;
	OrderForm *p, *q, *s, *prePoint;
	TCHAR szFltNum[16], szDate[16];

	GetDlgItemText(hwndDlg, IDC_FLIGHTNUM, szFltNum, 15);		//��ȡ�û����뺽��š���������
	GetDlgItemText(hwndDlg, IDC_DATE, szDate, 15);				

	if (!lstrlen(szFltNum)){									//δ���뺽���
		MessageBox(hwndDlg, TEXT("�����뺽��ţ�"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	DialogBox(hInst, MAKEINTRESOURCE(IDD_FLTNUM), NULL, (DLGPROC)FlightNumDlgProc);	//��ȡ��Ʊ��Ŀ

	if (!lstrlen(szBuffer))
		return FALSE;

	tickets_num = _ttoi(szBuffer);								//��Ʊ��

	p = passenger->OrderLink.head;
	prePoint = p;
	if (!p){													//�û��޶���
		MessageBox(hwndDlg, TEXT("��ǰ�û�û���Ѷ�Ʊ"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	while (p){													//��Ʊ������û���Ʊ����
		if (!lstrcmp(p->Flight_Number, szFltNum) && !lstrcmp(p->Date, szDate))
			num += p->Tickets_Num;
		p = p->psgNext;
	}
	if (tickets_num > num){										//��Ʊ�����ڸú����Ѷ�Ʊ��
		wsprintf(szBuffer, TEXT("�Բ��𣬸ú�����ֻ����%d�Ż�Ʊ����������д��Ʊ����"), num);
		MessageBox(hwndDlg, szBuffer, TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}//if

	p = passenger->OrderLink.head;
	while (p){													//��Ʊ
		if (!lstrcmp(p->Flight_Number, szFltNum) && !lstrcmp(p->Date, szDate)){
			if (tickets_num >= p->Tickets_Num){					//����Ʊ�����ڵ��ڵ�ǰ������Ʊ��

				s = p->psgNext;
				passenger->TicketNum -= p->Tickets_Num;			//�û���Ʊ������
				allorder.AllOrderNum--;							//�ܶ���������
				passenger->OrderLink.OrderNum--;				//�û�����������
				tickets_num -= p->Tickets_Num;					//��Ҫ��Ʊ������  ����Ҫ��Ʊ��Ϊ0�����˳����������ѭ��

				if (passenger->OrderLink.head == p){			//��pΪͷ��㣬ɾ��ͷ���
					passenger->OrderLink.head = p->psgNext;		//ͷָ��ָ����һ�����
					if (!p->psgNext)							//��pͬʱΪβ��㣬��βָ��ָ��NULL
						passenger->OrderLink.tail = NULL;
				}
				else if (!p->psgNext)							//��p��ͷ��㵫Ϊβ��㣬βָ��ָ��ǰһ�����
					passenger->OrderLink.tail = prePoint;
				prePoint->psgNext = p->psgNext;					//����ɾ�����p

				flight = flightlink.head;
				while (flight){									//������������
					if (!lstrcmp(flight->Flight_Number, szFltNum) && !lstrcmp(flight->Date, szDate)){
						flight->Vacant_Seat += p->Tickets_Num;
						break;
					}
					flight = flight->Next;
				}//while

				q = allorder.head;
				prePoint = q;
				while (q){										//ɾ�����������ж������
					if (q == p){								
						if (allorder.head == p){				//��pΪͷ��㣬ɾ��ͷ���
							allorder.head = p->Next;			
							if (!q->Next)						//��qͬʱΪβ���
								allorder.tail = NULL;
						}
						else if (!q->Next)						//��q��ͷ��㵫Ϊβ���
							allorder.tail = prePoint;
						prePoint->Next = q->Next;				 
						free(p); break;							//�ͷ�p����ڴ�
					}//if
					prePoint = q;
					q = q->Next;
				}//while

				if (tickets_num){								//����Ʊ����0���������Ʊ
					p = s; continue;
				}

				MessageBox(hwndDlg, TEXT("��Ʊ�ɹ���"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
				return TRUE;
			}//if
			else{												//��Ʊ��С�ڸö�����Ʊ��
				p->Tickets_Num -= tickets_num;					//������Ʊ������
				passenger->TicketNum -= tickets_num;			//�û���Ʊ������

				flight = flightlink.head;
				while (flight){
					if (!lstrcmp(flight->Flight_Number, szFltNum) && !lstrcmp(flight->Date, szDate)){
						flight->Vacant_Seat += tickets_num;		//������Ʊ����
						break;
					}
					flight = flight->Next;
				}//while

				MessageBox(hwndDlg, TEXT("��Ʊ�ɹ���"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
				return TRUE;									//��Ʊ�ɹ�
			}//else
		}//if
		prePoint = p;
		p = p->psgNext;
	}//while

	wsprintf(szBuffer, TEXT("�Բ�����û�ж��ú����Ʊ����ȷ����Ʊ����"));
	MessageBox(hwndDlg, szBuffer, TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
	return FALSE;
}//ReturnTickets()


//¼�뺽��
BOOL EntryFlight(HWND hEntryDlg){

	TCHAR szDiscount[16];
	TCHAR szFare[16], szSeat_Number[16];
	TCHAR szFlight_Number[16], szDate[16];
	TCHAR szDeparture[16], szDestination[16];
	TCHAR szTakeOff_Time[16], szLanding_Time[16];

	Flight *p = (Flight *)malloc(sizeof(Flight)), *q;
	if (!p){
		MessageBox(hEntryDlg, TEXT("�ڴ��������!"), TEXT("����"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	GetDlgItemText(hEntryDlg, IDC_DEPARTURE, szDeparture, 16);			//��ȡ¼�뺽����Ϣ
	GetDlgItemText(hEntryDlg, IDC_DESTINATION, szDestination, 16);
	GetDlgItemText(hEntryDlg, IDC_FLTNUM, szFlight_Number, 16);
	GetDlgItemText(hEntryDlg, IDC_FARE, szFare, 16);
	GetDlgItemText(hEntryDlg, IDC_TAKEOFFTIME, szTakeOff_Time, 16);
	GetDlgItemText(hEntryDlg, IDC_LANDINGTIME, szLanding_Time, 16);
	GetDlgItemText(hEntryDlg, IDC_SEATNUM, szSeat_Number, 16);
	GetDlgItemText(hEntryDlg, IDC_DISCOUNT, szDiscount, 16);
	GetDlgItemText(hEntryDlg, IDC_DATE, szDate, 16);

	if (!lstrlen(szDeparture) || !lstrlen(szDestination) || !lstrlen(szFlight_Number) || !lstrlen(szFare) ||
		!lstrlen(szTakeOff_Time) || !lstrlen(szLanding_Time) || !lstrlen(szSeat_Number))
	{
		MessageBox(hEntryDlg, TEXT("������д����������Ϣ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	q = flightlink.head;
	while (q){
		if (!lstrcmp(q->Flight_Number, szFlight_Number) && !lstrcmp(q->Date, szDate)){
			MessageBox(hEntryDlg, TEXT("�Բ���ͬһ������һ����ֻ����һ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
			free(p);
			return FALSE;
		}
		q = q->Next;
	}//while

	if (!lstrlen(szDiscount))
		wsprintf(szDiscount, "��");

	lstrcpy(p->Date, szDate);							//����¼�뺽������Ϣ
	lstrcpy(p->Discount, szDiscount);
	lstrcpy(p->Departure, szDeparture);
	lstrcpy(p->Destination, szDestination);
	lstrcpy(p->TakeOff_Time, szTakeOff_Time);
	lstrcpy(p->Landing_Time, szLanding_Time);
	lstrcpy(p->Flight_Number, szFlight_Number);
	
	p->Fare = _ttof(szFare);
	p->Seat_Number = _ttoi(szSeat_Number);
	p->Vacant_Seat = p->Seat_Number;
	p->Next = NULL;

	if (flightlink.head == NULL)						//���½��������������
		flightlink.head = p;
	else
		flightlink.tail->Next = p;
	flightlink.tail = p;
	flightlink.Flight_Number++;							//������Ŀ����

	MessageBox(hEntryDlg, TEXT("����¼��ɹ���"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);

	return TRUE;
}//EntryFlight()


//�޸ĺ�����Ϣ
BOOL ModifyFlight(HWND hModifyDlg){
	int seat_num;
	TCHAR szDiscount[16];
	TCHAR szFare[16], szSeat_Number[16];
	TCHAR szTakeOff_Time[16], szLanding_Time[16];
	TCHAR Mdf_flt_num[16], Mdf_flt_date[16];

	OrderForm *q;
	Flight *p;

	GetDlgItemText(hModifyDlg, IDC_MDFFLTNUM, Mdf_flt_num, 16);				//��ȡҪ�޸ĵĺ���š���������
	GetDlgItemText(hModifyDlg, IDC_MDFDATE, Mdf_flt_date, 16);

	if (!lstrlen(Mdf_flt_num)){
		MessageBox(hModifyDlg, TEXT("��������Ҫ�޸���Ϣ�ĺ����"), TEXT("��Ϣ"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	GetDlgItemText(hModifyDlg, IDC_FARE, szFare, 16);						//��ȡҪ�޸ĵ���Ϣ
	GetDlgItemText(hModifyDlg, IDC_DISCOUNT, szDiscount, 16);
	GetDlgItemText(hModifyDlg, IDC_SEATNUM, szSeat_Number, 16);
	GetDlgItemText(hModifyDlg, IDC_TAKEOFFTIME, szTakeOff_Time, 16);
	GetDlgItemText(hModifyDlg, IDC_LANDINGTIME, szLanding_Time, 16);

	p = flightlink.head;
	while (p){																//����Ҫ�޸ĵĺ���
		if (!lstrcmp(Mdf_flt_num, p->Flight_Number) && !lstrcmp(Mdf_flt_date, p->Date)){
			if (lstrlen(szFare))											//�ı亽��Ʊ��
				p->Fare = _ttof(szFare);
			if (lstrlen(szDiscount))										//�ı亽���ۿ�
				wsprintf(p->Discount, szDiscount);
			if (lstrlen(szSeat_Number)){									//�ı亽����λ��
				seat_num = _ttoi(szSeat_Number);
				if ((p->Seat_Number - seat_num) > p->Vacant_Seat){			//��������>=0
					MessageBox(hModifyDlg, TEXT("�Բ�����λ�����٣���������д"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
					return FALSE;
				}
				p->Vacant_Seat -= p->Seat_Number - seat_num;				//�ı���Ʊ����
				p->Seat_Number = seat_num;									//�ı���λ��
			}//if
			if (lstrlen(szTakeOff_Time)){									//�ı亽�����ʱ��
				wsprintf(p->TakeOff_Time, szTakeOff_Time);
				q = allorder.head;											
				while (q){													//�ı�ú������ж�������ʱ��
					if (!lstrcmp(q->Flight_Number, p->Flight_Number) && !lstrcmp(q->Date, p->Date))
						wsprintf(q->TakeOff_Time, szTakeOff_Time);
					q = q->Next;
				}
			}//if
			if (lstrlen(szLanding_Time)){									//�ı亽�ཱུ��ʱ��
				wsprintf(p->Landing_Time, szLanding_Time);
				q = allorder.head;											
				while (q) {													//�ı�ú������ж�������ʱ��
					if (!lstrcmp(q->Flight_Number, p->Flight_Number) && !lstrcmp(q->Date, p->Date))
						wsprintf(q->Landing_Time, szLanding_Time);
					q = q->Next;
				}
			}//if
			MessageBox(hModifyDlg, TEXT("�޸ĳɹ���"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
			return TRUE;
		}//if
		p = p->Next;
	}//while

	MessageBox(hModifyDlg, TEXT("�Բ�������Ҫ�޸ĵĺ�����δ¼�롣"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
	return FALSE;

}//ModifyFlight()


//�����˻�����
BOOL WriteAccountData(HWND hMainDlg){
	Passenger *p,*q;
	FILE *fp;
	if ((fp = fopen(".\\AccountData.txt", "w")) == NULL){
		MessageBox(hMainDlg, TEXT("�˻���Ϣ������ִ���!"), TEXT("����"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	p = psglink.head;
	while (p){
		fprintf(fp, "%s\n%s\n%s\n", p->Name, p->IdNum, p->PassWord);
		q = p;
		p = p->Next;
		free(q);
	}//while

	fclose(fp);
	return TRUE;
}//WriteAccountData


//���涩����Ϣ
BOOL WriteOrderData(HWND hMainDlg){
	OrderForm *p,*q;
	FILE *fp;
	if ((fp = fopen(".\\OrderForm.txt", "w")) == NULL){
		MessageBox(hMainDlg, TEXT("������Ϣ������ִ���!"), TEXT("����"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	p = allorder.head;
	while (p){
		fprintf(fp, "%s\n", p->IdNum);
		fprintf(fp, "%d\n", p->Order_Number);
		fprintf(fp, "%d\n", p->Tickets_Num);
		fprintf(fp, "%s\n", p->Flight_Number);
		fprintf(fp, "%s\n", p->Departure);
		fprintf(fp, "%s\n", p->Destination);
		fprintf(fp, "%s\n", p->Date);
		fprintf(fp, "%s\n", p->TakeOff_Time);
		fprintf(fp, "%s\n", p->Landing_Time);
		q = p;
		p = p->Next;
		free(q);
	}//while
	fclose(fp);
	return TRUE;
}//WriteOrderData()


//���溽����Ϣ
BOOL WriteFlightData(HWND hMainDlg){
	Flight *p,*q;
	FILE *fp;
	if ((fp = fopen(".\\FlightData.txt", "w")) == NULL){
		MessageBox(hMainDlg, TEXT("������Ϣ������ִ���!"), TEXT("����"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	p = flightlink.head;
	while (p){
		fprintf(fp, "%s\n", p->Flight_Number);
		fprintf(fp, "%.2lf\n", p->Fare);
		fprintf(fp, "%s\n", p->Discount);
		fprintf(fp, "%d\n%d\n", p->Seat_Number, p->Vacant_Seat);
		fprintf(fp, "%s\n", p->Departure);
		fprintf(fp, "%s\n", p->Destination);
		fprintf(fp, "%s\n", p->Date);
		fprintf(fp, "%s\n", p->TakeOff_Time);
		fprintf(fp, "%s\n", p->Landing_Time);
		q = p;
		p = p->Next;
		free(q);
	}//while
	fclose(fp);
	return TRUE;
}//WriteFlight()