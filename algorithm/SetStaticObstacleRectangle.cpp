// SetStaticObstacleRectangle.cpp : 实现文件
//

#include "stdafx.h"
#include "USV.h"
#include "SetStaticObstacleRectangle.h"
#include "SetStaticObstacleCircle.h"
#include "afxdialogex.h"
#include "USVDoc.h"
#include "USVView.h"
#include "math.h"
#include <afxdb.h>


// SetStaticObstacleRectangle 对话框

extern StaticOR staticor[20];

extern StartGoalPOINT startgoalp;
extern bool state_staticOR;//方形静态障碍物状态

extern int testNum;//测试次数
extern int thisPSOnum;//本次pso测试次数

IMPLEMENT_DYNAMIC(SetStaticObstacleRectangle, CDialogEx)

SetStaticObstacleRectangle::SetStaticObstacleRectangle(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetStaticObstacleRectangle::IDD, pParent)
	, CenterX(0)
	, CenterY(0)
	, DirectionA(0)
	, HalfLength(0)
	, HalfWidth(0)
{

}

SetStaticObstacleRectangle::~SetStaticObstacleRectangle()
{
}

void SetStaticObstacleRectangle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_StaticObstacleaRectangle, LIST_StaticObstacleRec);
	DDX_Text(pDX, IDC_StaticObstacleCX, CenterX);
	DDV_MinMaxUInt(pDX, CenterX, 0, 1200);
	DDX_Text(pDX, IDC_StaticObstacleCY, CenterY);
	DDV_MinMaxUInt(pDX, CenterY, 0, 800);
	DDX_Text(pDX, IDC_StaticObstacleDA, DirectionA);
	DDV_MinMaxFloat(pDX, DirectionA, 0, 360);
	DDX_Text(pDX, IDC_StaticObstacleHL, HalfLength);
	DDV_MinMaxUInt(pDX, HalfLength, 0, 1200);
	DDX_Text(pDX, IDC_StaticObstacleHW, HalfWidth);
	DDV_MinMaxUInt(pDX, HalfWidth, 0, 800);
}


BEGIN_MESSAGE_MAP(SetStaticObstacleRectangle, CDialogEx)
	ON_BN_CLICKED(IDAddObstacleR, &SetStaticObstacleRectangle::OnBnClickedAddobstacler)
	ON_BN_CLICKED(IDDeleteObstacleR, &SetStaticObstacleRectangle::OnBnClickedDeleteobstacler)
	ON_BN_CLICKED(IDOK, &SetStaticObstacleRectangle::OnBnClickedOk)
	ON_BN_CLICKED(IDSObstacleCInputR, &SetStaticObstacleRectangle::OnBnClickedSobstaclecinputr)
	ON_BN_CLICKED(IDSObstacleCOutputR, &SetStaticObstacleRectangle::OnBnClickedSobstaclecoutputr)
END_MESSAGE_MAP()


// SetStaticObstacleRectangle 消息处理程序




BOOL SetStaticObstacleRectangle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString Field[6] = { "序号", "中心点X", "中心点Y","半长", "半宽","方向角" };//列表视的表头
	LIST_StaticObstacleRec.InsertColumn(0, Field[0], LVCFMT_CENTER, 41);
	for (int j = 1; j<6; j++)
	{
		LIST_StaticObstacleRec.InsertColumn(j, Field[j], LVCFMT_CENTER, 60);				//插入表头标题
	}
	//初始化输入框	
	CenterX = 0;
	CenterY = 0;
	DirectionA = 0;
	HalfLength = 0;
	HalfWidth = 0;
	
	//读入静态障碍物（方）数据--添加始末点更改后的 极径，极角 更改
	for (UINT i = 0; i < 20; i++)
	{
		if (staticor[i].CenterPm != 0)
		{
			UpdateData(TRUE);

			int num = LIST_StaticObstacleRec.GetItemCount();

			CString stemp[6];
			stemp[0].Format("%d", num + 1);
			stemp[1].Format("%d", staticor[i].CenterX);
			stemp[2].Format("%d", staticor[i].CenterY);
			stemp[3].Format("%d", staticor[i].HalfL);
			stemp[4].Format("%d", staticor[i].HalfW);
			stemp[5].Format("%0.3f", staticor[i].Da);
			LIST_StaticObstacleRec.InsertItem(num, stemp[0]);		//插入行
			for (int j = 1; j < 6; j++)
			{
				LIST_StaticObstacleRec.SetItemText(num, j, stemp[j]);					//设置该行的不同列的显示字符
			}
			UpdateData(false);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void SetStaticObstacleRectangle::OnBnClickedAddobstacler()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (HalfLength != 0)
	{
		int num = LIST_StaticObstacleRec.GetItemCount();
		staticor[num].CenterX = CenterX;
		staticor[num].CenterY = CenterY;
		staticor[num].HalfL = HalfLength;
		staticor[num].HalfW = HalfWidth;
		staticor[num].Da = DirectionA;

		CalculateStaticOR(num);//计算方形障碍物数据
		
		CString stemp[6];
		stemp[0].Format("%d", num + 1);
		stemp[1].Format("%d", CenterX);
		stemp[2].Format("%d", CenterY);
		stemp[3].Format("%d", HalfLength);
		stemp[4].Format("%d", HalfWidth);
		stemp[5].Format("%0.3f", DirectionA);

		LIST_StaticObstacleRec.InsertItem(num, stemp[0]);		//插入行
		for (int i = 1; i<6; i++)
		{
			LIST_StaticObstacleRec.SetItemText(num, i, stemp[i]);					//设置该行的不同列的显示字符
		}
		
		CenterX = 0;
		CenterY = 0;
		DirectionA = 0;
		HalfLength = 0;
		HalfWidth = 0;
	}

	UpdateData(false);

}


void SetStaticObstacleRectangle::OnBnClickedDeleteobstacler()
{
	// TODO:  在此添加控件通知处理程序代码
	int num = LIST_StaticObstacleRec.GetItemCount();
	while (LIST_StaticObstacleRec.DeleteItem(num - 1));
	staticor[num-1] = { 0 };
}


void SetStaticObstacleRectangle::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	testNum = 0;//测试次数
	thisPSOnum = 0;//本次pso测试次数
	
	int num = LIST_StaticObstacleRec.GetItemCount();

	//对staticor数组按中心点极径值  进行从小到大排序
	for (int j = 0; j < num - 1; j++)
	{
		for (int i = 0; i < num - 1 - j; i++)
		{
			StaticOR temp;
			if (staticor[i].CenterPm>staticor[i + 1].CenterPm)
			{
				temp = staticor[i];
				staticor[i] = staticor[i + 1];
				staticor[i + 1] = temp;
			}
		}
	}

	if (staticor[0].HalfL != 0)
	{
		state_staticOR = 1;
	}
	CDialogEx::OnOK();
}


void SetStaticObstacleRectangle::CalculateStaticOR(int i)//计算方形障碍物数据
{
	SetStaticObstacleCircle temp;
	staticor[i].CenterPm = sqrt(pow((float)abs(int(startgoalp.sx - staticor[i].CenterX)), 2) + pow((float)abs(int(startgoalp.sy - staticor[i].CenterY)), 2));//极径
	staticor[i].CenterPa = temp.calculate_PolarAngle((float)staticor[i].CenterX, (float)staticor[i].CenterY);//极角

	int tempX, tempY;
	float tempPA = 0.0 - staticor[i].Da * 2 * pi / 360;
	//局部坐标系中0的顶点(逆时针排序)
	tempX = staticor[i].HalfL;
	tempY = staticor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	staticor[i].Peak[0].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)staticor[i].CenterX;
	staticor[i].Peak[0].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)staticor[i].CenterY;
	staticor[i].Peak[0].Pm = sqrt(pow((float)abs((float)startgoalp.sx - staticor[i].Peak[0].x), 2) + pow((float)abs((float)startgoalp.sy - staticor[i].Peak[0].y), 2));//极径;
	staticor[i].Peak[0].Pa = temp.calculate_PolarAngle(staticor[i].Peak[0].x, staticor[i].Peak[0].y);//极角

	//局部坐标系中0的顶点(扩展坐标)
	tempX = staticor[i].HalfL + safe_expandT;
	tempY = staticor[i].HalfW + safe_expandT;
	//从局部坐标系转换到全局坐标系
	staticor[i].Peak[0].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)staticor[i].CenterX;
	staticor[i].Peak[0].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)staticor[i].CenterY;
	staticor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - staticor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - staticor[i].Peak[0].yEX), 2));//极径;
	staticor[i].Peak[0].PaEX = temp.calculate_PolarAngle(staticor[i].Peak[0].xEX, staticor[i].Peak[0].yEX);//极角

	//局部坐标系中1的顶点
	tempX = staticor[i].HalfL;
	tempY = 0 - staticor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	staticor[i].Peak[1].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)staticor[i].CenterX;
	staticor[i].Peak[1].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)staticor[i].CenterY;
	staticor[i].Peak[1].Pm = sqrt(pow((float)abs((float)startgoalp.sx - staticor[i].Peak[1].x), 2) + pow((float)abs((float)startgoalp.sy - staticor[i].Peak[1].y), 2));//极径;
	staticor[i].Peak[1].Pa = temp.calculate_PolarAngle(staticor[i].Peak[1].x, staticor[i].Peak[1].y);//极角

	//局部坐标系中1的顶点(扩展坐标)
	tempX = staticor[i].HalfL + safe_expandT;
	tempY = 0 - staticor[i].HalfW - safe_expandT;
	//从局部坐标系转换到全局坐标系
	staticor[i].Peak[1].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)staticor[i].CenterX;
	staticor[i].Peak[1].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)staticor[i].CenterY;
	staticor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - staticor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - staticor[i].Peak[1].yEX), 2));//极径;
	staticor[i].Peak[1].PaEX = temp.calculate_PolarAngle(staticor[i].Peak[1].xEX, staticor[i].Peak[1].yEX);//极角

	//局部坐标系中2的顶点
	tempX = 0 - staticor[i].HalfL;
	tempY = 0 - staticor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	staticor[i].Peak[2].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)staticor[i].CenterX;
	staticor[i].Peak[2].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)staticor[i].CenterY;
	staticor[i].Peak[2].Pm = sqrt(pow((float)abs((float)startgoalp.sx - staticor[i].Peak[2].x), 2) + pow((float)abs((float)startgoalp.sy - staticor[i].Peak[2].y), 2));//极径;
	staticor[i].Peak[2].Pa = temp.calculate_PolarAngle(staticor[i].Peak[2].x, staticor[i].Peak[2].y);//极角

	//局部坐标系中2的顶点(扩展坐标)
	tempX = 0 - staticor[i].HalfL - safe_expandT;
	tempY = 0 - staticor[i].HalfW - safe_expandT;
	//从局部坐标系转换到全局坐标系
	staticor[i].Peak[2].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)staticor[i].CenterX;
	staticor[i].Peak[2].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)staticor[i].CenterY;
	staticor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - staticor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - staticor[i].Peak[2].yEX), 2));//极径;
	staticor[i].Peak[2].PaEX = temp.calculate_PolarAngle(staticor[i].Peak[2].xEX, staticor[i].Peak[2].yEX);//极角

	//局部坐标系中3的顶点
	tempX = 0 - staticor[i].HalfL;
	tempY = staticor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	staticor[i].Peak[3].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)staticor[i].CenterX;
	staticor[i].Peak[3].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)staticor[i].CenterY;
	staticor[i].Peak[3].Pm = sqrt(pow((float)abs((float)startgoalp.sx - staticor[i].Peak[3].x), 2) + pow((float)abs((float)startgoalp.sy - staticor[i].Peak[3].y), 2));//极径;
	staticor[i].Peak[3].Pa = temp.calculate_PolarAngle(staticor[i].Peak[3].x, staticor[i].Peak[3].y);//极角

	//局部坐标系中3的顶点(扩展坐标)
	tempX = 0 - staticor[i].HalfL - safe_expandT;
	tempY = staticor[i].HalfW + safe_expandT;
	//从局部坐标系转换到全局坐标系
	staticor[i].Peak[3].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)staticor[i].CenterX;
	staticor[i].Peak[3].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)staticor[i].CenterY;
	staticor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - staticor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - staticor[i].Peak[3].yEX), 2));//极径;
	staticor[i].Peak[3].PaEX = temp.calculate_PolarAngle(staticor[i].Peak[3].xEX, staticor[i].Peak[3].yEX);//极角

	//对四个顶点按pm进行排序
	//找出最小值的下标
	float min = 0.0;
	int flag = 0;
	for (int m = 0; m < 4; m++)
	{
		if (m == 0)
		{
			min = staticor[i].Peak[0].Pm;
			flag = 0;
		}
		else
		{
			if (min>staticor[i].Peak[m].Pm)
			{
				min = staticor[i].Peak[m].Pm;
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
		tempPeak = staticor[i].Peak[0];
		staticor[i].Peak[0] = staticor[i].Peak[1];
		staticor[i].Peak[1] = staticor[i].Peak[2];
		staticor[i].Peak[2] = staticor[i].Peak[3];
		staticor[i].Peak[3] = tempPeak;
	}
	else if (flag == 2)
	{
		tempPeak = staticor[i].Peak[0];
		staticor[i].Peak[0] = staticor[i].Peak[2];
		staticor[i].Peak[2] = tempPeak;
		tempPeak = staticor[i].Peak[3];
		staticor[i].Peak[3] = staticor[i].Peak[1];
		staticor[i].Peak[1] = tempPeak;
	}
	else if (flag == 3)
	{
		tempPeak = staticor[i].Peak[0];
		staticor[i].Peak[0] = staticor[i].Peak[3];
		staticor[i].Peak[3] = staticor[i].Peak[2];
		staticor[i].Peak[2] = staticor[i].Peak[1];
		staticor[i].Peak[1] = tempPeak;
	}
}

void SetStaticObstacleRectangle::OnBnClickedSobstaclecinputr()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, //TRUE或FALSE。TRUE为打开文件；FALSE为保存文件
		"xls", //为缺省的扩展名
		"FileList", //为显示在文件名组合框的编辑框的文件名，一般可选NULL 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,//为对话框风格，一般为OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,即隐藏只读选项和覆盖已有文件前提示。 
		"Excel 文件(*.xls)|*.xls||"//为下拉列表枢中显示文件类型
		);
	dlg.m_ofn.lpstrTitle = "导入静态障碍物（方）数据";

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
	LIST_StaticObstacleRec.DeleteAllItems();

	//AfxMessageBox("OK");
	/* sSql = "SELECT 学号,姓名,成绩 "
	"FROM EXCELDEMO";      */
	// "ORDER BY 姓名";
	sSql.Format("SELECT 序号,中心点X, 中心点Y, 半长, 半宽, 方向角 FROM 静态障碍物（方）数据"); 
	pset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
	while (!pset.IsEOF())
	{
		pset.GetFieldValue("序号", arr[0]);//前面字段必须与表中的相同，否则出错。
		pset.GetFieldValue("中心点X", arr[1]);
		pset.GetFieldValue("中心点Y", arr[2]);
		pset.GetFieldValue("半长", arr[3]);
		pset.GetFieldValue("半宽", arr[4]);
		pset.GetFieldValue("方向角", arr[5]);

		int count = LIST_StaticObstacleRec.GetItemCount();//插入到ListCtrl中
		LIST_StaticObstacleRec.InsertItem(count, arr[0]);
		LIST_StaticObstacleRec.SetItemText(count, 1, arr[1]);
		LIST_StaticObstacleRec.SetItemText(count, 2, arr[2]);
		LIST_StaticObstacleRec.SetItemText(count, 3, arr[3]);
		LIST_StaticObstacleRec.SetItemText(count, 4, arr[4]);
		LIST_StaticObstacleRec.SetItemText(count, 5, arr[5]);

		pset.MoveNext();
	}
	db.Close();

	int num = LIST_StaticObstacleRec.GetItemCount();

	for (int i = 0; i < 20; i++)
	{
	staticor[i] = { 0 };//清零
	}
	//把列表数据添加进数组staticor[]
	for (int i = 0; i < num; i++)
	{
	staticor[i].CenterX = atoi(LIST_StaticObstacleRec.GetItemText(i, 1));
	staticor[i].CenterY = atoi(LIST_StaticObstacleRec.GetItemText(i, 2));
	staticor[i].HalfL = atoi(LIST_StaticObstacleRec.GetItemText(i, 3));
	staticor[i].HalfW = atoi(LIST_StaticObstacleRec.GetItemText(i, 4));
	staticor[i].Da = atof(LIST_StaticObstacleRec.GetItemText(i, 5));
	CalculateStaticOR(i);//计算方形障碍物数据
	}

	MessageBox("Excel数据成功导入系统!", "导入成功");
}


void SetStaticObstacleRectangle::OnBnClickedSobstaclecoutputr()
{
	// TODO:  在此添加控件通知处理程序代码
	if (LIST_StaticObstacleRec.GetItemCount() <= 0)
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
	dlg.m_ofn.lpstrTitle = "USV静态障碍物（方）数据另存为";

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

			iColumnNum = LIST_StaticObstacleRec.GetHeaderCtrl()->GetItemCount();
			iRowCount = LIST_StaticObstacleRec.GetItemCount();

			sSql = " CREATE TABLE 静态障碍物（方）数据 ( ";
			strInsert = " INSERT INTO 静态障碍物（方）数据 ( ";
			//获得列标题名称
			lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
			lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
			lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
			//以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
			for (i = 0; i< iColumnNum; i++)
			{
				if (!(LIST_StaticObstacleRec.GetColumn(i, &lvCol)))
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
					LIST_StaticObstacleRec.GetItemText(j, i, chTemp, 33);
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

		AfxMessageBox("保存静态障碍物（方）数据为Excel文件成功！");
	}
	CATCH_ALL(e)
	{
		//错误类型很多，根据需要进行报错。
		AfxMessageBox("Excel文件保存失败。");
	}
	END_CATCH_ALL;
}

PointInRC SetStaticObstacleRectangle::calculate_pointInRC(REC Peak0, REC Peak1, REC Peak2, REC Peak3, float x, float y)//计算点在矩形的那个位置（共9个）
{
	float y01, y12, y23, y30, tempX1, tempX2, tempY1, tempY2;
	PointInRC pointInRC = {};

	//显示坐标系下计算，返回极坐标系下值
	//01
	tempX1 = Peak0.xEX;
	tempX2 = Peak1.xEX;
	tempY1 = Peak0.yEX;
	tempY2 = Peak1.yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y01 = x - tempX1;
	}
	else
	{
		y01 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//12
	tempX1 = Peak1.xEX;
	tempX2 = Peak2.xEX;
	tempY1 = Peak1.yEX;
	tempY2 = Peak2.yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y12 = x - tempX1;
	}
	else
	{
		y12 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//23
	tempX1 = Peak2.xEX;
	tempX2 = Peak3.xEX;
	tempY1 = Peak2.yEX;
	tempY2 = Peak3.yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y23 = x - tempX1;
	}
	else
	{
		y23 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//30
	tempX1 = Peak3.xEX;
	tempX2 = Peak0.xEX;
	tempY1 = Peak3.yEX;
	tempY2 = Peak0.yEX;
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
