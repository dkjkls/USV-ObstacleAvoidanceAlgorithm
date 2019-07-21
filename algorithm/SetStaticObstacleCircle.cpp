// SetStaticObstacleCircle.cpp : 实现文件
//

#include "stdafx.h"
#include "SetStaticObstacleCircle.h"
#include "afxdialogex.h"
#include "USVDoc.h"
#include "USVView.h"
#include "math.h"
#include <afxdb.h>

// SetStaticObstacleCircle 对话框

extern StaticOC staticoc[20];
extern StartGoalPOINT startgoalp;
extern bool state_staticOC;//圆形静态障碍物状态

extern int testNum;//测试次数
extern int thisPSOnum;//本次pso测试次数


IMPLEMENT_DYNAMIC(SetStaticObstacleCircle, CDialogEx)

SetStaticObstacleCircle::SetStaticObstacleCircle(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetStaticObstacleCircle::IDD, pParent)
	, StaticObstacleX(0)
	, StaticObstacleY(0)
	, StaticObstacleR(0)
{

}

SetStaticObstacleCircle::~SetStaticObstacleCircle()
{
}

void SetStaticObstacleCircle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_StaticObstacle, LIST_StaticObstacle);
	DDX_Text(pDX, IDC_StaticObstacleX, StaticObstacleX);
	DDV_MinMaxUInt(pDX, StaticObstacleX, 0, 1200);
	DDX_Text(pDX, IDC_StaticObstacleY, StaticObstacleY);
	DDV_MinMaxUInt(pDX, StaticObstacleY, 0, 800);
	DDX_Text(pDX, IDC_StaticObstacleR, StaticObstacleR);
	DDV_MinMaxUInt(pDX, StaticObstacleR, 0, 1200);
}


BEGIN_MESSAGE_MAP(SetStaticObstacleCircle, CDialogEx)

	ON_BN_CLICKED(IDAddObstacleC, &SetStaticObstacleCircle::OnBnClickedAddobstaclec)
	ON_BN_CLICKED(IDDeleteObstacleC, &SetStaticObstacleCircle::OnBnClickedDeleteobstaclec)
	ON_BN_CLICKED(IDOK, &SetStaticObstacleCircle::OnBnClickedOk)
	ON_EN_CHANGE(IDC_StaticObstacleY, &SetStaticObstacleCircle::OnEnChangeStaticobstacley)
	ON_BN_CLICKED(IDSObstacleCInput, &SetStaticObstacleCircle::OnBnClickedSobstaclecinput)
	ON_BN_CLICKED(IDSObstacleCOutput, &SetStaticObstacleCircle::OnBnClickedSobstaclecoutput)
END_MESSAGE_MAP()


// SetStaticObstacleCircle 消息处理程序



BOOL SetStaticObstacleCircle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString Field[6] = { "序号", "X坐标", "Y坐标", "最大宽度","极径","极角" };						//列表视的表头
	LIST_StaticObstacle.InsertColumn(0, Field[0], LVCFMT_CENTER, 41);
	for (int j = 1; j<6; j++)
	{
		LIST_StaticObstacle.InsertColumn(j, Field[j], LVCFMT_CENTER, 60);				//插入表头标题
	}
	//初始化输入框	
	StaticObstacleX = 0;
	StaticObstacleY = 0;
	StaticObstacleR = 0;

	//读入静态障碍物（圆）数据--添加始末点更改后的 极径，极角 更改
	for (UINT i = 0; i < 20; i++)
	{
		if (staticoc[i].r != 0)
		{
			UpdateData(TRUE);

			int num = LIST_StaticObstacle.GetItemCount();

			CString stemp[6];
			stemp[0].Format("%d", num+1);
			stemp[1].Format("%d", staticoc[i].x);
			stemp[2].Format("%d", staticoc[i].y);
			stemp[3].Format("%d", staticoc[i].r);
			float pm = sqrt(pow((float)abs(int(startgoalp.sx - staticoc[i].x)), 2) + pow((float)abs(int(startgoalp.sy - staticoc[i].y)), 2));//极径
			float pa = calculate_PolarAngle((float)staticoc[i].x, (float)staticoc[i].y);//极角
			stemp[4].Format("%0.3f", pm);
			stemp[5].Format("%0.3f", pa);
			LIST_StaticObstacle.InsertItem(num, stemp[0]);		//插入行
			for (int j = 1; j < 6; j++)
			{
				LIST_StaticObstacle.SetItemText(num, j, stemp[j]);					//设置该行的不同列的显示字符
			}
			UpdateData(false);
		}
	}
	return TRUE;
	// return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void SetStaticObstacleCircle::OnBnClickedAddobstaclec()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if (StaticObstacleR!=0)
	{
		int num = LIST_StaticObstacle.GetItemCount();
		
		float temp = sqrt(pow((float)abs(int(startgoalp.sx - StaticObstacleX)), 2) + pow((float)abs(int(startgoalp.sy - StaticObstacleY)), 2));//极径

		CString stemp[6];
		stemp[0].Format("%d", num + 1);
		stemp[1].Format("%d", StaticObstacleX);
		stemp[2].Format("%d", StaticObstacleY);
		stemp[3].Format("%d", StaticObstacleR);
		stemp[4].Format("%0.3f", temp);
		stemp[5].Format("%0.3f", calculate_PolarAngle((float)StaticObstacleX, (float)StaticObstacleY));

		LIST_StaticObstacle.InsertItem(num, stemp[0]);		//插入行
		for (int i = 1; i<6; i++)
		{
			LIST_StaticObstacle.SetItemText(num, i, stemp[i]);					//设置该行的不同列的显示字符
		}
		StaticObstacleX = 0;
		StaticObstacleY = 0;
		StaticObstacleR = 0;
	}
	/*else
	{
		AfxMessageBox("最大宽度应大于0！");
		return;
	}*/
	UpdateData(false);
}


void SetStaticObstacleCircle::OnBnClickedDeleteobstaclec()
{
	// TODO:  在此添加控件通知处理程序代码
	int num = LIST_StaticObstacle.GetItemCount();
	while (LIST_StaticObstacle.DeleteItem(num-1));
}


void SetStaticObstacleCircle::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	testNum = 0;//测试次数
	thisPSOnum = 0;//本次pso测试次数

	int num = LIST_StaticObstacle.GetItemCount();

	for (int i = 0; i < 20; i++)
	{
		staticoc[i] = { 0 };//清零
	}
	//把列表数据添加进数组staticoc[]
	for (int i = 0; i < num; i++)
	{
		staticoc[i].x = atoi(LIST_StaticObstacle.GetItemText(i, 1));
		staticoc[i].y = atoi(LIST_StaticObstacle.GetItemText(i, 2));
		staticoc[i].r = atoi(LIST_StaticObstacle.GetItemText(i, 3));
		staticoc[i].pm = atof(LIST_StaticObstacle.GetItemText(i, 4));
		staticoc[i].pa = atof(LIST_StaticObstacle.GetItemText(i, 5));
	}
	
	//对staticoc数组按极径值  进行从小到大排序
	for (int j = 0; j < num-1; j++)
	{
		for (int i = 0; i < num - 1-j; i++)
		{
			StaticOC temp;
			if (staticoc[i].pm>staticoc[i+1].pm)
			{
				temp = staticoc[i];
				staticoc[i] = staticoc[i + 1];
				staticoc[i + 1] = temp;
			}
		}
	}

	if (staticoc[0].r != 0)
	{
		state_staticOC = 1;
	}
	CDialogEx::OnOK();
}


void SetStaticObstacleCircle::OnEnChangeStaticobstacley()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void SetStaticObstacleCircle::OnBnClickedSobstaclecinput()
{
	// TODO:  在此添加控件通知处理程序代码

	CFileDialog dlg(TRUE, //TRUE或FALSE。TRUE为打开文件；FALSE为保存文件
		"xls", //为缺省的扩展名
		"FileList", //为显示在文件名组合框的编辑框的文件名，一般可选NULL 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,//为对话框风格，一般为OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,即隐藏只读选项和覆盖已有文件前提示。 
		"Excel 文件(*.xls)|*.xls||"//为下拉列表枢中显示文件类型
		);
	dlg.m_ofn.lpstrTitle = "导入静态障碍物（圆）数据";

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
	LIST_StaticObstacle.DeleteAllItems();

	//AfxMessageBox("OK");
	/* sSql = "SELECT 学号,姓名,成绩 "
	"FROM EXCELDEMO";      */
	// "ORDER BY 姓名";
	sSql.Format("SELECT 序号,X坐标,Y坐标,最大宽度,极径,极角 FROM 静态障碍物（圆）数据");
	pset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
	while (!pset.IsEOF())
	{
		pset.GetFieldValue("序号", arr[0]);//前面字段必须与表中的相同，否则出错。
		pset.GetFieldValue("X坐标", arr[1]);
		pset.GetFieldValue("Y坐标", arr[2]);
		pset.GetFieldValue("最大宽度", arr[3]);

		UINT staticOCX = atoi(arr[1]);
		UINT staticOCY = atoi(arr[2]);
		float pm = sqrt(pow((float)abs(int(startgoalp.sx - staticOCX)), 2) + pow((float)abs(int(startgoalp.sy - staticOCY)), 2));//极径
		float pa = calculate_PolarAngle((float)staticOCX, (float)staticOCY);//极角
		arr[4].Format("%0.3f", pm);
		arr[5].Format("%0.3f", pa);

		int count = LIST_StaticObstacle.GetItemCount();//插入到ListCtrl中
		LIST_StaticObstacle.InsertItem(count, arr[0]);
		LIST_StaticObstacle.SetItemText(count, 1, arr[1]);
		LIST_StaticObstacle.SetItemText(count, 2, arr[2]);
		LIST_StaticObstacle.SetItemText(count, 3, arr[3]);
		LIST_StaticObstacle.SetItemText(count, 4, arr[4]);
		LIST_StaticObstacle.SetItemText(count, 5, arr[5]);
		
		pset.MoveNext();
	}
	db.Close();
	
	MessageBox("Excel数据成功导入系统!", "导入成功");
}


void SetStaticObstacleCircle::OnBnClickedSobstaclecoutput()
{
	// TODO:  在此添加控件通知处理程序代码
	//首先判断列表框中是否有记录
	if (LIST_StaticObstacle.GetItemCount() <= 0)
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
	dlg.m_ofn.lpstrTitle = "静态障碍物（圆）数据另存为";

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

			iColumnNum = LIST_StaticObstacle.GetHeaderCtrl()->GetItemCount();
			iRowCount = LIST_StaticObstacle.GetItemCount();

			sSql = " CREATE TABLE 静态障碍物（圆）数据 ( ";
			strInsert = " INSERT INTO 静态障碍物（圆）数据 ( ";
			//获得列标题名称
			lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
			lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
			lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
			//以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
			for (i = 0; i< iColumnNum; i++)
			{
				if (!(LIST_StaticObstacle.GetColumn(i, &lvCol)))
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
					LIST_StaticObstacle.GetItemText(j, i, chTemp, 33);
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

		AfxMessageBox("保存静态障碍物（圆）数据为Excel文件成功！");
	}
	CATCH_ALL(e)
	{
		//错误类型很多，根据需要进行报错。
		AfxMessageBox("Excel文件保存失败。");
	}
	END_CATCH_ALL;
}


float SetStaticObstacleCircle::calculate_PolarAngle(float StaticObstacleX, float StaticObstacleY)//显示坐标下的转换
{
	float temp2 = atan(float(((float)startgoalp.gy - (float)startgoalp.sy) / ((float)startgoalp.gx - (float)startgoalp.sx)));//始末点连线角度
	float temp3, temp4;
	temp3 = atan(abs(float(((float)StaticObstacleY - (float)startgoalp.sy)) / (abs((float)StaticObstacleX - (float)startgoalp.sx))));//GDI角度

	float k1 = (float(startgoalp.gy - startgoalp.sy) / (startgoalp.gx - startgoalp.sx));//始末点斜率

	if (k1 == 0)
	{
		if (StaticObstacleY > startgoalp.sy)//在始末点之上
		{
			if (StaticObstacleX >= startgoalp.sx)//第四象限
			{
				temp4 = 2 * pi - temp3;
			}
			else//第三象限
			{
				temp4 = pi + temp3;
			}

		}
		else//在始末点之下
		{
			if (StaticObstacleX >= startgoalp.sx)//第一象限
			{
				temp4 = temp3;
			}
			else//第二象限
			{
				temp4 = pi - temp3;
			}
		}
	}
	else
	{
		float k2 = -1 / k1;//垂直线斜率

		if (StaticObstacleY > (k1*(StaticObstacleX - startgoalp.sx) + startgoalp.sy))//在始末点之上
		{
			if (StaticObstacleY >= (k2*(StaticObstacleX - startgoalp.sx) + startgoalp.sy))//第四象限
			{
				temp4 = 2 * pi - temp3 - temp2;
			}
			else//第三象限
			{
				temp4 = pi + temp3 + temp2;
			}

		}
		else//在始末点之下
		{
			if (StaticObstacleY >= (k2*(StaticObstacleX - startgoalp.sx) + startgoalp.sy))//第一象限
			{
				temp4 = temp3 + temp2;
			}
			else//第二象限
			{
				temp4 = pi - temp3 - temp2;
			}
		}
	}
	return temp4;
}
