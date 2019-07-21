// SetDynamicOC.cpp : 实现文件
//

#include "stdafx.h"
#include "USV.h"
#include "USVDoc.h"
#include "USVView.h"
#include "SetDynamicOC.h"
#include "afxdialogex.h"
#include "SetStaticObstacleCircle.h"
#include "math.h"
#include <afxdb.h>

extern DynamicOC dynamicoc[20];
extern StartGoalPOINT startgoalp;
extern bool state_dynamicOC;//圆形动态障碍物状态
extern int testNum;//测试次数
extern int thisPSOnum;//本次pso测试次数

SetStaticObstacleCircle calculate_PA;

// SetDynamicOC 对话框

IMPLEMENT_DYNAMIC(SetDynamicOC, CDialogEx)

SetDynamicOC::SetDynamicOC(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetDynamicOC::IDD, pParent)
	, DynamicOC_X(0)
	, DynamicOC_Y(0)
	, DynamicOC_R(0)
	, DynamicOC_DA(0)
	, DynamicOC_V(0)
{

}

SetDynamicOC::~SetDynamicOC()
{
}

void SetDynamicOC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DynamicOC_X, DynamicOC_X);
	DDV_MinMaxUInt(pDX, DynamicOC_X, 0, 2000);
	DDX_Text(pDX, IDC_DynamicOC_Y, DynamicOC_Y);
	DDV_MinMaxUInt(pDX, DynamicOC_Y, 0, 2000);
	DDX_Text(pDX, IDC_DynamicOC_R, DynamicOC_R);
	DDV_MinMaxUInt(pDX, DynamicOC_R, 0, 2000);
	DDX_Text(pDX, IDC_DynamicOC_DA, DynamicOC_DA);
	//DDV_MinMaxUInt(pDX, DynamicOC_DA, 0.0, 360.0);
	DDX_Text(pDX, IDC_DynamicOC_V, DynamicOC_V);
	//DDV_MinMaxUInt(pDX, DynamicOC_V, 0.0, 200.0);
	DDX_Control(pDX, IDC_LIST_DynamicOC, LIST_DynamicOC);
}


BEGIN_MESSAGE_MAP(SetDynamicOC, CDialogEx)
	ON_BN_CLICKED(IDAddDynamicOC, &SetDynamicOC::OnBnClickedAddDynamicoc)
	ON_BN_CLICKED(IDDeleteDynamicOC, &SetDynamicOC::OnBnClickedDeletedynamicoc)
	ON_BN_CLICKED(IDOK, &SetDynamicOC::OnBnClickedOk)
	ON_BN_CLICKED(IDSDynamicOCIutput, &SetDynamicOC::OnBnClickedSdynamicociutput)
	ON_BN_CLICKED(IDSDynamicOCOutputC, &SetDynamicOC::OnBnClickedSdynamicocoutputc)
END_MESSAGE_MAP()


// SetDynamicOC 消息处理程序


BOOL SetDynamicOC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString Field[6] = { "序号", "X坐标", "Y坐标", "最大宽度", "方向角", "速度" };	//列表视的表头
	LIST_DynamicOC.InsertColumn(0, Field[0], LVCFMT_CENTER, 40);
	for (int j = 1; j<6; j++)
	{
		LIST_DynamicOC.InsertColumn(j, Field[j], LVCFMT_CENTER, 61);				//插入表头标题
	}
	//初始化输入框	
	DynamicOC_X = 0;
	DynamicOC_Y = 0;
	DynamicOC_R = 0;
	DynamicOC_DA = 0.0;
	DynamicOC_V = 0.0;

	//读入静态障碍物（圆）数据--添加始末点更改后的 极径，极角 更改
	for (UINT i = 0; i < 20; i++)
	{
		if (dynamicoc[i].r != 0)
		{
			UpdateData(TRUE);

			int num = LIST_DynamicOC.GetItemCount();

			CString stemp[6];
			stemp[0].Format("%d", num + 1);
			stemp[1].Format("%d", dynamicoc[i].x);
			stemp[2].Format("%d", dynamicoc[i].y);
			stemp[3].Format("%d", dynamicoc[i].r);
			stemp[4].Format("%0.3f", dynamicoc[i].Da);
			stemp[5].Format("%0.3f", dynamicoc[i].V);

			LIST_DynamicOC.InsertItem(num, stemp[0]);		//插入行
			for (int j = 1; j < 6; j++)
			{
				LIST_DynamicOC.SetItemText(num, j, stemp[j]);					//设置该行的不同列的显示字符
			}
			UpdateData(false);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void SetDynamicOC::OnBnClickedAddDynamicoc()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (DynamicOC_R != 0)
	{
		int num = LIST_DynamicOC.GetItemCount();

		CString stemp[6];
		stemp[0].Format("%d", num + 1);
		stemp[1].Format("%d", DynamicOC_X);
		stemp[2].Format("%d", DynamicOC_Y);
		stemp[3].Format("%d", DynamicOC_R);
		stemp[4].Format("%0.3f", DynamicOC_DA);
		stemp[5].Format("%0.3f", DynamicOC_V);

		LIST_DynamicOC.InsertItem(num, stemp[0]);		//插入行
		for (int i = 1; i<6; i++)
		{
			LIST_DynamicOC.SetItemText(num, i, stemp[i]);					//设置该行的不同列的显示字符
		}
		DynamicOC_X = 0;
		DynamicOC_Y = 0;
		DynamicOC_R = 0;
		DynamicOC_DA = 0.0;
		DynamicOC_V = 0.0;
	}

	UpdateData(false);
}

void SetDynamicOC::OnBnClickedDeletedynamicoc()
{
	// TODO:  在此添加控件通知处理程序代码
	int num = LIST_DynamicOC.GetItemCount();
	while (LIST_DynamicOC.DeleteItem(num - 1));
}

void SetDynamicOC::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	testNum = 0;//测试次数
	thisPSOnum = 0;//本次pso测试次数

	int num = LIST_DynamicOC.GetItemCount();

	for (int i = 0; i < 20; i++)
	{
		dynamicoc[i] = { 0 };//清零
	}
	//把列表数据添加进数组staticoc[]
	for (int i = 0; i < num; i++)
	{
		dynamicoc[i].x = atoi(LIST_DynamicOC.GetItemText(i, 1));
		dynamicoc[i].y = atoi(LIST_DynamicOC.GetItemText(i, 2));
		dynamicoc[i].r = atoi(LIST_DynamicOC.GetItemText(i, 3));
		dynamicoc[i].Da = atof(LIST_DynamicOC.GetItemText(i, 4));
		dynamicoc[i].V = atof(LIST_DynamicOC.GetItemText(i, 5));

		dynamicoc[i].pm = sqrt(pow((float)abs(int(startgoalp.sx - dynamicoc[i].x)), 2) + pow((float)abs(int(startgoalp.sy - dynamicoc[i].y)), 2));//极径
		dynamicoc[i].pa = calculate_PA.calculate_PolarAngle((float)dynamicoc[i].x, (float)dynamicoc[i].y);//极角
	}

	//对staticoc数组按极径值  进行从小到大排序
	for (int j = 0; j < num - 1; j++)
	{
		for (int i = 0; i < num - 1 - j; i++)
		{
			DynamicOC temp;
			if (dynamicoc[i].pm>dynamicoc[i + 1].pm)
			{
				temp = dynamicoc[i];
				dynamicoc[i] = dynamicoc[i + 1];
				dynamicoc[i + 1] = temp;
			}
		}
	}

	if (dynamicoc[0].r != 0)
	{
		state_dynamicOC = 1;
	}

	CDialogEx::OnOK();
}


void SetDynamicOC::OnBnClickedSdynamicociutput()
{
	// TODO:  在此添加控件通知处理程序代码

	CFileDialog dlg(TRUE, //TRUE或FALSE。TRUE为打开文件；FALSE为保存文件
		"xls", //为缺省的扩展名
		"FileList", //为显示在文件名组合框的编辑框的文件名，一般可选NULL 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,//为对话框风格，一般为OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,即隐藏只读选项和覆盖已有文件前提示。 
		"Excel 文件(*.xls)|*.xls||"//为下拉列表枢中显示文件类型
		);
	dlg.m_ofn.lpstrTitle = "导入动态障碍物（圆）数据";

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
	CString sSql, arr[6];

	sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, strFilePath, strFilePath);
	if (!db.OpenEx(sSql, CDatabase::noOdbcDialog))//连接数据源DJB．xls
	{
		MessageBox("打开EXCEL文件失败!", "错误");
		return;
	}
	//打开EXCEL表
	CRecordset pset(&db);
	LIST_DynamicOC.DeleteAllItems();

	//AfxMessageBox("OK");
	/* sSql = "SELECT 学号,姓名,成绩 "
	"FROM EXCELDEMO";      */
	// "ORDER BY 姓名";
	sSql.Format("SELECT 序号,X坐标,Y坐标,最大宽度, 方向角, 速度 ,极径, 极角 FROM 动态障碍物（圆）数据");
	pset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
	while (!pset.IsEOF())
	{
		pset.GetFieldValue("序号", arr[0]);//前面字段必须与表中的相同，否则出错。
		pset.GetFieldValue("X坐标", arr[1]);
		pset.GetFieldValue("Y坐标", arr[2]);
		pset.GetFieldValue("最大宽度", arr[3]);
		pset.GetFieldValue("方向角", arr[4]);
		pset.GetFieldValue("速度", arr[5]);

		int count = LIST_DynamicOC.GetItemCount();//插入到ListCtrl中
		LIST_DynamicOC.InsertItem(count, arr[0]);
		LIST_DynamicOC.SetItemText(count, 1, arr[1]);
		LIST_DynamicOC.SetItemText(count, 2, arr[2]);
		LIST_DynamicOC.SetItemText(count, 3, arr[3]);
		LIST_DynamicOC.SetItemText(count, 4, arr[4]);
		LIST_DynamicOC.SetItemText(count, 5, arr[5]);

		pset.MoveNext();
	}
	db.Close();

	MessageBox("Excel数据成功导入系统!", "导入成功");

}

void SetDynamicOC::OnBnClickedSdynamicocoutputc()
{
	// TODO:  在此添加控件通知处理程序代码
	if (LIST_DynamicOC.GetItemCount() <= 0)
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
	dlg.m_ofn.lpstrTitle = "动态障碍物（圆）数据另存为";

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

			iColumnNum = LIST_DynamicOC.GetHeaderCtrl()->GetItemCount();
			iRowCount = LIST_DynamicOC.GetItemCount();

			sSql = " CREATE TABLE 动态障碍物（圆）数据 ( ";
			strInsert = " INSERT INTO 动态障碍物（圆）数据 ( ";
			//获得列标题名称
			lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
			lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
			lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
			//以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
			for (i = 0; i< iColumnNum; i++)
			{
				if (!(LIST_DynamicOC.GetColumn(i, &lvCol)))
					return;
				if (i<iColumnNum - 1)
				{
					sSql = sSql + lvCol.pszText + " TEXT , ";
					strInsert = strInsert + lvCol.pszText + " , ";
				}
				else
				{
					sSql = sSql + lvCol.pszText + " TEXT , " + "极径 TEXT," + "极角 TEXT)";
					strInsert = strInsert + lvCol.pszText + ", 极径 ," + "极角 " + " )  VALUES ( ";
				}
			}

			//创建Excel表格文件
			database.ExecuteSQL(sSql);

			//循环提取记录并插入到EXCEL中
			sSql = strInsert;
			char chTemp[33];
			int tempx, tempy;
			float tempPm, tempPa;
			for (j = 0; j<iRowCount; j++)
			{
				memset(chTemp, 0, 33);
				for (i = 0; i<iColumnNum; i++)
				{
					LIST_DynamicOC.GetItemText(j, i, chTemp, 33);
					if (i < (iColumnNum - 1))
					{
						sSql = sSql + "'" + chTemp + "' , ";
					}
					else
					{
						//sSql = sSql + "'" + chTemp + "' ) ";
						
						tempx = atoi(LIST_DynamicOC.GetItemText(j, 1));
						tempy = atoi(LIST_DynamicOC.GetItemText(j, 2));
						tempPm = sqrt(pow((float)abs(int(startgoalp.sx - tempx)), 2) + pow((float)abs(int(startgoalp.sy - tempy)), 2));//极径
						tempPa = calculate_PA.calculate_PolarAngle((float)tempx, (float)tempy);//极角
						CString stemp[2];
						stemp[0].Format("%f", tempPm);
						stemp[1].Format("%f", tempPa);
						
						sSql = sSql + "'" + chTemp + "' , " + "'" + stemp[0] + "' , " + "'" + stemp[1] + "' ) ";
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

		AfxMessageBox("保存动态障碍物（圆）数据为Excel文件成功！");
	}
	CATCH_ALL(e)
	{
		//错误类型很多，根据需要进行报错。
		AfxMessageBox("Excel文件保存失败。");
	}
	END_CATCH_ALL;
}
