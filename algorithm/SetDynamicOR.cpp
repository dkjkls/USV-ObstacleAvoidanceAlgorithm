// SetDynamicOR.cpp : 实现文件
//

#include "stdafx.h"
#include "USV.h"
#include "SetDynamicOR.h"
#include "afxdialogex.h"
#include "SetStaticObstacleCircle.h"
#include "SetStaticObstacleRectangle.h"
#include "afxdialogex.h"
#include "USVDoc.h"
#include "USVView.h"
#include "math.h"
#include <afxdb.h>

extern DynamicOR dynamicor[20];
extern StartGoalPOINT startgoalp;
extern bool state_dynamicOR;//方形静态障碍物状态
extern int testNum;//测试次数
extern int thisPSOnum;//本次pso测试次数

// SetDynamicOR 对话框

IMPLEMENT_DYNAMIC(SetDynamicOR, CDialogEx)

SetDynamicOR::SetDynamicOR(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetDynamicOR::IDD, pParent)
	, DynamicOR_X(0)
	, DynamicOR_Y(0)
	, DynamicOR_HL(0)
	, DynamicOR_HW(0)
	, DynamicOR_DA(0)
	, DynamicOR_V(0)
{

}

SetDynamicOR::~SetDynamicOR()
{
}

void SetDynamicOR::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DynamicOR_X, DynamicOR_X);
	DDV_MinMaxUInt(pDX, DynamicOR_X, 0, 2000);
	DDX_Text(pDX, IDC_DynamicOR_Y, DynamicOR_Y);
	DDV_MinMaxUInt(pDX, DynamicOR_Y, 0, 2000);
	DDX_Text(pDX, IDC_DynamicOR_HL, DynamicOR_HL);
	DDV_MinMaxUInt(pDX, DynamicOR_HL, 0, 2000);
	DDX_Text(pDX, IDC_DynamicOR_HW, DynamicOR_HW);
	DDV_MinMaxUInt(pDX, DynamicOR_HW, 0, 2000);
	DDX_Text(pDX, IDC_DynamicOR_DA, DynamicOR_DA);
	DDV_MinMaxFloat(pDX, DynamicOR_DA, 0, 360);
	DDX_Text(pDX, IDC_DynamicOR_V, DynamicOR_V);
	DDV_MinMaxFloat(pDX, DynamicOR_V, 0, 200);
	DDX_Control(pDX, IDC_LIST_DynamicOR, LIST_DynamicOR);
}


BEGIN_MESSAGE_MAP(SetDynamicOR, CDialogEx)
	ON_BN_CLICKED(IDAddDynamicOR, &SetDynamicOR::OnBnClickedAdddynamicOR)
	ON_BN_CLICKED(IDDeleteDynamicOR, &SetDynamicOR::OnBnClickedDeletedynamicOR)
	ON_BN_CLICKED(IDOK, &SetDynamicOR::OnBnClickedOk)
	ON_BN_CLICKED(IDSDynamicORInput, &SetDynamicOR::OnBnClickedSdynamicorinput)
	ON_BN_CLICKED(IDSDynamicOROutput, &SetDynamicOR::OnBnClickedSdynamicoroutput)
END_MESSAGE_MAP()


// SetDynamicOR 消息处理程序


BOOL SetDynamicOR::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString Field[7] = { "序号", "中心点X", "中心点Y", "半长", "半宽", "方向角", "速度" };//列表视的表头
	LIST_DynamicOR.InsertColumn(0, Field[0], LVCFMT_CENTER, 41);
	for (int j = 1; j<7; j++)
	{
		LIST_DynamicOR.InsertColumn(j, Field[j], LVCFMT_CENTER, 63);				//插入表头标题
	}
	//初始化输入框	
	DynamicOR_X = 0;
	DynamicOR_Y = 0;
	DynamicOR_HL = 0;
	DynamicOR_HW = 0;
	DynamicOR_DA = 0;
	DynamicOR_V = 0;

	//读入静态障碍物（方）数据--添加始末点更改后的 极径，极角 更改
	for (UINT i = 0; i < 20; i++)
	{
		if (dynamicor[i].CenterPm != 0)
		{
			UpdateData(TRUE);

			int num = LIST_DynamicOR.GetItemCount();

			CString stemp[7];
			stemp[0].Format("%d", num + 1);
			stemp[1].Format("%d", dynamicor[i].CenterX);
			stemp[2].Format("%d", dynamicor[i].CenterY);
			stemp[3].Format("%d", dynamicor[i].HalfL);
			stemp[4].Format("%d", dynamicor[i].HalfW);
			stemp[5].Format("%0.3f", dynamicor[i].Da);
			stemp[6].Format("%0.3f", dynamicor[i].V);
			LIST_DynamicOR.InsertItem(num, stemp[0]);		//插入行
			for (int j = 1; j < 7; j++)
			{
				LIST_DynamicOR.SetItemText(num, j, stemp[j]);					//设置该行的不同列的显示字符
			}
			UpdateData(false);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void SetDynamicOR::OnBnClickedAdddynamicOR()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if (DynamicOR_HL != 0)
	{
		int num = LIST_DynamicOR.GetItemCount();
		dynamicor[num].CenterX = DynamicOR_X;
		dynamicor[num].CenterY = DynamicOR_Y;
		dynamicor[num].HalfL = DynamicOR_HL;
		dynamicor[num].HalfW = DynamicOR_HW;
		dynamicor[num].Da = DynamicOR_DA;
		dynamicor[num].V = DynamicOR_V;

		CalculateDynamicOR(num);//计算方形障碍物数据

		CString stemp[7];
		stemp[0].Format("%d", num + 1);
		stemp[1].Format("%d", DynamicOR_X);
		stemp[2].Format("%d", DynamicOR_Y);
		stemp[3].Format("%d", DynamicOR_HL);
		stemp[4].Format("%d", DynamicOR_HW);
		stemp[5].Format("%0.3f", DynamicOR_DA);
		stemp[6].Format("%0.3f", DynamicOR_V);

		LIST_DynamicOR.InsertItem(num, stemp[0]);		//插入行
		for (int i = 1; i<7; i++)
		{
			LIST_DynamicOR.SetItemText(num, i, stemp[i]);					//设置该行的不同列的显示字符
		}

		DynamicOR_X = 0;
		DynamicOR_Y = 0;
		DynamicOR_HL = 0;
		DynamicOR_HW = 0;
		DynamicOR_DA = 0;
		DynamicOR_V = 0;
	}

	UpdateData(false);
}

void SetDynamicOR::OnBnClickedDeletedynamicOR()
{
	// TODO:  在此添加控件通知处理程序代码
	int num = LIST_DynamicOR.GetItemCount();
	while (LIST_DynamicOR.DeleteItem(num - 1));
	dynamicor[num - 1] = { 0 };
}

void SetDynamicOR::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	testNum = 0;//测试次数
	thisPSOnum = 0;//本次pso测试次数

	int num = LIST_DynamicOR.GetItemCount();

	//对dynamicor数组按中心点极径值  进行从小到大排序
	for (int j = 0; j < num - 1; j++)
	{
		for (int i = 0; i < num - 1 - j; i++)
		{
			DynamicOR temp;
			if (dynamicor[i].CenterPm>dynamicor[i + 1].CenterPm)
			{
				temp = dynamicor[i];
				dynamicor[i] = dynamicor[i + 1];
				dynamicor[i + 1] = temp;
			}
		}
	}

	if (dynamicor[0].HalfL != 0)
	{
		state_dynamicOR = 1;
	}
	
	CDialogEx::OnOK();
}

void SetDynamicOR::CalculateDynamicOR(int i)//计算方形障碍物数据
{
	SetStaticObstacleCircle temp;
	dynamicor[i].CenterPm = sqrt(pow((float)abs(int(startgoalp.sx - dynamicor[i].CenterX)), 2) + pow((float)abs(int(startgoalp.sy - dynamicor[i].CenterY)), 2));//极径
	dynamicor[i].CenterPa = temp.calculate_PolarAngle((float)dynamicor[i].CenterX, (float)dynamicor[i].CenterY);//极角

	int tempX, tempY;
	float tempPA = 0.0 - dynamicor[i].Da * 2 * pi / 360;
	//局部坐标系中0的顶点(逆时针排序)
	tempX = dynamicor[i].HalfL;
	tempY = dynamicor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	dynamicor[i].Peak[0].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)dynamicor[i].CenterX;
	dynamicor[i].Peak[0].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)dynamicor[i].CenterY;
	dynamicor[i].Peak[0].Pm = sqrt(pow((float)abs((float)startgoalp.sx - dynamicor[i].Peak[0].x), 2) + pow((float)abs((float)startgoalp.sy - dynamicor[i].Peak[0].y), 2));//极径;
	dynamicor[i].Peak[0].Pa = temp.calculate_PolarAngle(dynamicor[i].Peak[0].x, dynamicor[i].Peak[0].y);//极角

	//局部坐标系中0的顶点(扩展坐标)
	tempX = dynamicor[i].HalfL + safe_expandT;
	tempY = dynamicor[i].HalfW + safe_expandT;
	//从局部坐标系转换到全局坐标系
	dynamicor[i].Peak[0].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)dynamicor[i].CenterX;
	dynamicor[i].Peak[0].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)dynamicor[i].CenterY;
	dynamicor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - dynamicor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - dynamicor[i].Peak[0].yEX), 2));//极径;
	dynamicor[i].Peak[0].PaEX = temp.calculate_PolarAngle(dynamicor[i].Peak[0].xEX, dynamicor[i].Peak[0].yEX);//极角

	//局部坐标系中1的顶点
	tempX = dynamicor[i].HalfL;
	tempY = 0 - dynamicor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	dynamicor[i].Peak[1].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)dynamicor[i].CenterX;
	dynamicor[i].Peak[1].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)dynamicor[i].CenterY;
	dynamicor[i].Peak[1].Pm = sqrt(pow((float)abs((float)startgoalp.sx - dynamicor[i].Peak[1].x), 2) + pow((float)abs((float)startgoalp.sy - dynamicor[i].Peak[1].y), 2));//极径;
	dynamicor[i].Peak[1].Pa = temp.calculate_PolarAngle(dynamicor[i].Peak[1].x, dynamicor[i].Peak[1].y);//极角

	//局部坐标系中1的顶点(扩展坐标)
	tempX = dynamicor[i].HalfL + safe_expandT;
	tempY = 0 - dynamicor[i].HalfW - safe_expandT;
	//从局部坐标系转换到全局坐标系
	dynamicor[i].Peak[1].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)dynamicor[i].CenterX;
	dynamicor[i].Peak[1].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)dynamicor[i].CenterY;
	dynamicor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - dynamicor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - dynamicor[i].Peak[1].yEX), 2));//极径;
	dynamicor[i].Peak[1].PaEX = temp.calculate_PolarAngle(dynamicor[i].Peak[1].xEX, dynamicor[i].Peak[1].yEX);//极角

	//局部坐标系中2的顶点
	tempX = 0 - dynamicor[i].HalfL;
	tempY = 0 - dynamicor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	dynamicor[i].Peak[2].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)dynamicor[i].CenterX;
	dynamicor[i].Peak[2].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)dynamicor[i].CenterY;
	dynamicor[i].Peak[2].Pm = sqrt(pow((float)abs((float)startgoalp.sx - dynamicor[i].Peak[2].x), 2) + pow((float)abs((float)startgoalp.sy - dynamicor[i].Peak[2].y), 2));//极径;
	dynamicor[i].Peak[2].Pa = temp.calculate_PolarAngle(dynamicor[i].Peak[2].x, dynamicor[i].Peak[2].y);//极角

	//局部坐标系中2的顶点(扩展坐标)
	tempX = 0 - dynamicor[i].HalfL - safe_expandT;
	tempY = 0 - dynamicor[i].HalfW - safe_expandT;
	//从局部坐标系转换到全局坐标系
	dynamicor[i].Peak[2].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)dynamicor[i].CenterX;
	dynamicor[i].Peak[2].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)dynamicor[i].CenterY;
	dynamicor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - dynamicor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - dynamicor[i].Peak[2].yEX), 2));//极径;
	dynamicor[i].Peak[2].PaEX = temp.calculate_PolarAngle(dynamicor[i].Peak[2].xEX, dynamicor[i].Peak[2].yEX);//极角

	//局部坐标系中3的顶点
	tempX = 0 - dynamicor[i].HalfL;
	tempY = dynamicor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	dynamicor[i].Peak[3].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)dynamicor[i].CenterX;
	dynamicor[i].Peak[3].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)dynamicor[i].CenterY;
	dynamicor[i].Peak[3].Pm = sqrt(pow((float)abs((float)startgoalp.sx - dynamicor[i].Peak[3].x), 2) + pow((float)abs((float)startgoalp.sy - dynamicor[i].Peak[3].y), 2));//极径;
	dynamicor[i].Peak[3].Pa = temp.calculate_PolarAngle(dynamicor[i].Peak[3].x, dynamicor[i].Peak[3].y);//极角

	//局部坐标系中3的顶点(扩展坐标)
	tempX = 0 - dynamicor[i].HalfL - safe_expandT;
	tempY = dynamicor[i].HalfW + safe_expandT;
	//从局部坐标系转换到全局坐标系
	dynamicor[i].Peak[3].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)dynamicor[i].CenterX;
	dynamicor[i].Peak[3].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)dynamicor[i].CenterY;
	dynamicor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - dynamicor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - dynamicor[i].Peak[3].yEX), 2));//极径;
	dynamicor[i].Peak[3].PaEX = temp.calculate_PolarAngle(dynamicor[i].Peak[3].xEX, dynamicor[i].Peak[3].yEX);//极角

	//对四个顶点按pm进行排序
	//找出最小值的下标
	float min = 0.0;
	int flag = 0;
	for (int m = 0; m < 4; m++)
	{
		if (m == 0)
		{
			min = dynamicor[i].Peak[0].Pm;
			flag = 0;
		}
		else
		{
			if (min>dynamicor[i].Peak[m].Pm)
			{
				min = dynamicor[i].Peak[m].Pm;
				flag = m;
			}
		}
	}
	//进行排序
	REC tempPeak;
	if (flag == 0)
	{
	}
	else if (flag == 1)
	{
		tempPeak = dynamicor[i].Peak[0];
		dynamicor[i].Peak[0] = dynamicor[i].Peak[1];
		dynamicor[i].Peak[1] = dynamicor[i].Peak[2];
		dynamicor[i].Peak[2] = dynamicor[i].Peak[3];
		dynamicor[i].Peak[3] = tempPeak;
	}
	else if (flag == 2)
	{
		tempPeak = dynamicor[i].Peak[0];
		dynamicor[i].Peak[0] = dynamicor[i].Peak[2];
		dynamicor[i].Peak[2] = tempPeak;
		tempPeak = dynamicor[i].Peak[3];
		dynamicor[i].Peak[3] = dynamicor[i].Peak[1];
		dynamicor[i].Peak[1] = tempPeak;
	}
	else if (flag == 3)
	{
		tempPeak = dynamicor[i].Peak[0];
		dynamicor[i].Peak[0] = dynamicor[i].Peak[3];
		dynamicor[i].Peak[3] = dynamicor[i].Peak[2];
		dynamicor[i].Peak[2] = dynamicor[i].Peak[1];
		dynamicor[i].Peak[1] = tempPeak;
	}
}

void SetDynamicOR::OnBnClickedSdynamicorinput()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, //TRUE或FALSE。TRUE为打开文件；FALSE为保存文件
		"xls", //为缺省的扩展名
		"FileList", //为显示在文件名组合框的编辑框的文件名，一般可选NULL 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,//为对话框风格，一般为OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,即隐藏只读选项和覆盖已有文件前提示。 
		"Excel 文件(*.xls)|*.xls||"//为下拉列表枢中显示文件类型
		);
	dlg.m_ofn.lpstrTitle = "导入动态障碍物（方）数据";

	if (dlg.DoModal() != IDOK)
		return;
	CString strFilePath;
	//获得文件路径名
	strFilePath = dlg.GetPathName();
	//判断文件是否已经存在，存在则打开文件
	DWORD dwRe = GetFileAttributes(strFilePath);
	if (dwRe != (DWORD)-1)
	{
		//ShellExecute(NULL, NULL, strFilePath, NULL, NULL, SW_RESTORE); 
	}
	else return;

	CDatabase db;//数据库库需要包含头文件 #include <afxdb.h>
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel驱动
	CString sSql, arr[7];

	sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, strFilePath, strFilePath);
	if (!db.OpenEx(sSql, CDatabase::noOdbcDialog))//连接数据源DJB．xls
	{
		MessageBox("打开EXCEL文件失败!", "错误");
		return;
	}
	//打开EXCEL表
	CRecordset pset(&db);
	LIST_DynamicOR.DeleteAllItems();

	//AfxMessageBox("OK");
	/* sSql = "SELECT 学号,姓名,成绩 "
	"FROM EXCELDEMO";      */
	// "ORDER BY 姓名";
	sSql.Format("SELECT 序号,中心点X, 中心点Y, 半长, 半宽, 方向角, 速度 FROM 动态障碍物（方）数据");
	pset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
	while (!pset.IsEOF())
	{
		pset.GetFieldValue("序号", arr[0]);//前面字段必须与表中的相同，否则出错。
		pset.GetFieldValue("中心点X", arr[1]);
		pset.GetFieldValue("中心点Y", arr[2]);
		pset.GetFieldValue("半长", arr[3]);
		pset.GetFieldValue("半宽", arr[4]);
		pset.GetFieldValue("方向角", arr[5]);
		pset.GetFieldValue("速度", arr[6]);

		int count = LIST_DynamicOR.GetItemCount();//插入到ListCtrl中
		LIST_DynamicOR.InsertItem(count, arr[0]);
		LIST_DynamicOR.SetItemText(count, 1, arr[1]);
		LIST_DynamicOR.SetItemText(count, 2, arr[2]);
		LIST_DynamicOR.SetItemText(count, 3, arr[3]);
		LIST_DynamicOR.SetItemText(count, 4, arr[4]);
		LIST_DynamicOR.SetItemText(count, 5, arr[5]);
		LIST_DynamicOR.SetItemText(count, 6, arr[6]);

		pset.MoveNext();
	}
	db.Close();

	int num = LIST_DynamicOR.GetItemCount();

	for (int i = 0; i < 20; i++)
	{
		dynamicor[i] = { 0 };//清零
	}
	//把列表数据添加进数组dynamicor[]
	for (int i = 0; i < num; i++)
	{
		dynamicor[i].CenterX = atoi(LIST_DynamicOR.GetItemText(i, 1));
		dynamicor[i].CenterY = atoi(LIST_DynamicOR.GetItemText(i, 2));
		dynamicor[i].HalfL = atoi(LIST_DynamicOR.GetItemText(i, 3));
		dynamicor[i].HalfW = atoi(LIST_DynamicOR.GetItemText(i, 4));
		dynamicor[i].Da = atof(LIST_DynamicOR.GetItemText(i, 5));
		dynamicor[i].V = atof(LIST_DynamicOR.GetItemText(i, 6));
		CalculateDynamicOR(i);//计算方形障碍物数据
	}

	MessageBox("Excel数据成功导入系统!", "导入成功");
}

void SetDynamicOR::OnBnClickedSdynamicoroutput()
{
	// TODO:  在此添加控件通知处理程序代码
	if (LIST_DynamicOR.GetItemCount() <= 0)
	{
		AfxMessageBox("列表中没有记录需要保存！");
		return;
	}
	//打开另存为对话框 ，需要包含 #include <Afxdlgs.h>
	CFileDialog dlg(FALSE,
		"xls",
		"FileList",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Excel 文件(*.xls)|*.xls||");
	dlg.m_ofn.lpstrTitle = "USV动态障碍物（方）数据另存为";

	if (dlg.DoModal() != IDOK)
		return;
	CString strFilePath;
	//获得文件路径名
	strFilePath = dlg.GetPathName();
	//判断文件是否已经存在，存在则删除重建
	DWORD dwRe = GetFileAttributes(strFilePath);
	if (dwRe != (DWORD)-1)
	{
		DeleteFile(strFilePath);
	}

	CDatabase database;//数据库库需要包含头文件 #include <afxdb.h>
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel驱动
	CString sSql, strInsert;

	TRY
	{
		// 创建进行存取的字符串
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, strFilePath, strFilePath);

		// 创建数据库 (既Excel表格文件)
		if (database.OpenEx(sSql, CDatabase::noOdbcDialog))
		{
			//获得列别框总列数
			int iColumnNum, iRowCount;
			LVCOLUMN lvCol;
			CString strColName; //用于保存列标题名称
			int i, j; //列、行循环参数

			iColumnNum = LIST_DynamicOR.GetHeaderCtrl()->GetItemCount();
			iRowCount = LIST_DynamicOR.GetItemCount();

			sSql = " CREATE TABLE 动态障碍物（方）数据 ( ";
			strInsert = " INSERT INTO 动态障碍物（方）数据 ( ";
			//获得列标题名称
			lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
			lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
			lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
			//以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
			for (i = 0; i< iColumnNum; i++)
			{
				if (!(LIST_DynamicOR.GetColumn(i, &lvCol)))
					return;
				if (i<iColumnNum - 1)
				{
					sSql = sSql + lvCol.pszText + " TEXT , ";
					strInsert = strInsert + lvCol.pszText + " , ";
				}
				else
				{
					sSql = sSql + lvCol.pszText + " TEXT ) ";
					strInsert = strInsert + lvCol.pszText + " )  VALUES ( ";
				}
			}
			//创建Excel表格文件
			database.ExecuteSQL(sSql);

			//循环提取记录并插入到EXCEL中
			sSql = strInsert;
			char chTemp[33];
			for (j = 0; j<iRowCount; j++)
			{
				memset(chTemp, 0, 33);
				for (i = 0; i<iColumnNum; i++)
				{
					LIST_DynamicOR.GetItemText(j, i, chTemp, 33);
					if (i < (iColumnNum - 1))
					{
						sSql = sSql + "'" + chTemp + "' , ";
					}
					else
					{
						sSql = sSql + "'" + chTemp + "' ) ";
					}
				}
				//将记录插入到表格中
				database.ExecuteSQL(sSql);
				sSql = strInsert;
			}

			sSql = " CREATE TABLE 始末点 ( 起点X TEXT , 起点Y TEXT , 终点X TEXT , 终点Y TEXT ) ";
			//创建Excel表格文件
			database.ExecuteSQL(sSql);

			CString stemp[4];
			stemp[0].Format("%d", startgoalp.sx);
			stemp[1].Format("%d", startgoalp.sy);
			stemp[2].Format("%d", startgoalp.gx);
			stemp[3].Format("%d", startgoalp.gy);
			strInsert = " INSERT INTO 始末点 (起点X , 起点Y , 终点X , 终点Y )  VALUES ( ";
			strInsert = strInsert + "'" + stemp[0] + "' , " + "'" + stemp[1] + "' , " + "'" + stemp[2] + "' , " + "'" + stemp[3] + "') ";
			database.ExecuteSQL(strInsert);
		}

		// 关闭Excel表格文件
		database.Close();

		AfxMessageBox("保存动态障碍物（方）数据为Excel文件成功！");
	}
	CATCH_ALL(e)
	{
		//错误类型很多，根据需要进行报错。
		AfxMessageBox("Excel文件保存失败。");
	}
	END_CATCH_ALL;
}

PointInRC SetDynamicOR::calculate_pointInDRC(int m, float x, float y)//计算点在矩形的那个位置（共9个）
{
	float y01, y12, y23, y30, tempX1, tempX2, tempY1, tempY2;
	PointInRC pointInRC;

	//显示坐标系下计算，返回极坐标系下值
	//01
	tempX1 = dynamicor[m].Peak[0].xEX;
	tempX2 = dynamicor[m].Peak[1].xEX;
	tempY1 = dynamicor[m].Peak[0].yEX;
	tempY2 = dynamicor[m].Peak[1].yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y01 = x - tempX1;
	}
	else
	{
		y01 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//12
	tempX1 = dynamicor[m].Peak[1].xEX;
	tempX2 = dynamicor[m].Peak[2].xEX;
	tempY1 = dynamicor[m].Peak[1].yEX;
	tempY2 = dynamicor[m].Peak[2].yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y12 = x - tempX1;
	}
	else
	{
		y12 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//23
	tempX1 = dynamicor[m].Peak[2].xEX;
	tempX2 = dynamicor[m].Peak[3].xEX;
	tempY1 = dynamicor[m].Peak[2].yEX;
	tempY2 = dynamicor[m].Peak[3].yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y23 = x - tempX1;
	}
	else
	{
		y23 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//30
	tempX1 = dynamicor[m].Peak[3].xEX;
	tempX2 = dynamicor[m].Peak[0].xEX;
	tempY1 = dynamicor[m].Peak[3].yEX;
	tempY2 = dynamicor[m].Peak[0].yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y30 = x - tempX1;
	}
	else
	{
		y30 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	if ((y01 >= 0) & (y12 >= 0) & (y30 <= 0) & (y23 <= 0))
	{
		pointInRC.point = 5;//在矩形内部
	}
	else if ((y01 < 0) & (y30 > 0))
	{
		pointInRC.point = 0;
		pointInRC.min = 1;
		pointInRC.max = 3;
	}
	else if ((y01 < 0) & (y12 < 0))
	{
		pointInRC.point = 1;
		pointInRC.min = 2;
		pointInRC.max = 0;
	}
	else if ((y12 < 0) & (y23 > 0))
	{
		pointInRC.point = 2;
		pointInRC.min = 3;
		pointInRC.max = 1;
	}
	else if ((y23 > 0) & (y30 > 0))
	{
		pointInRC.point = 3;
		pointInRC.min = 0;
		pointInRC.max = 2;
	}
	else if ((y01 < 0) & (y12 >= 0) & (y30 <= 0))
	{
		pointInRC.point = 10;
		pointInRC.min = 1;
		pointInRC.max = 0;
	}
	else if ((y01 >= 0) & (y12 < 0) & (y23 <= 0))
	{
		pointInRC.point = 12;
		pointInRC.min = 2;
		pointInRC.max = 1;
	}
	else if ((y12 >= 0) & (y23 > 0) & (y30 <= 0))
	{
		pointInRC.point = 23;
		pointInRC.min = 3;
		pointInRC.max = 2;
	}
	else if ((y01 >= 0) & (y23 <= 0) & (y30 > 0))
	{
		pointInRC.point = 30;
		pointInRC.min = 0;
		pointInRC.max = 3;
	}
	return pointInRC;
}