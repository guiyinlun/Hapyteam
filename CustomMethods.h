#pragma once
#include <Core/api.h>

class CHAPYTreeCtrl:public CBCGPTreeCtrlEx
{ 
public:
	virtual void InitColors();
};
class CHAPYListCtrl:public CBCGPGridCtrl
{
public:
	CHAPYListCtrl();
	virtual void InitColors();
};

namespace customMethods{
	//Tree-------------------------------------------------------------------------------------------------------
	//创建项目树
	void createProjectTree(int projectid,CHAPYTreeCtrl* Tree,CBCGPBreadcrumb* breadcrumb);
	//添加数据到项目树目录和路径栏
	void addDataToProjectTreeAndBreadcrumb(ProjectDir* dir, st_exec_errors& error,HTREEITEM treeChild,HBREADCRUMBITEM breadhChild,CHAPYTreeCtrl* Tree,CBCGPBreadcrumb* breadcrumb);
	//添加目录到附加文件树
	void addDirsToAttachedFileTree(vector<AttachmentFile*> AttachmentFiles,CBCGPGridRow* pchild,CHAPYTreeCtrl* tree); 
	//添加数据到引用列表
	void addDataToReferenceList( int thisid,CHAPYTreeCtrl* tree,CBCGPComboBox* combo,int combonum);

	//List------------------------------------------------------------------------------------------------------
	//根据文件夹Id添加子目录
	void addSubDirsToFolderList(int folderId, ProjectDir* dir,CHAPYListCtrl* List);
	//根据文件夹Id添加文件
	void addFilesToFolderList(int thisid,int projectid,CHAPYListCtrl* List);
	//添加版本文件
	void addDataToVersionList(int thisid,CHAPYListCtrl* List);
	//添加文件到附加文件列表
	void addFilesToAttachedFileList(int thisid,int projectid,CListCtrl& List,std::string thisname);
	//添加数据到文件属性列表
	void addDataToFilePropertiesList(CBCGPPropList* m_propertylist,CHAPYListCtrl* List);
	//FolderList更新
	void updateFolderList(int thisid,int projectid,CHAPYListCtrl* List);

	//更新FileDetailsLayoutView
	void updateFileDetailsByTabNum(int tabnum,int thisid);
	//打开目录选择窗口
	std::string openDirSelectDlg();
	//从路径中提取文件名
	CString getFileNameByPath(CString strPath);
	//从文件名中提取后缀名
	string getFileSuffixByFileName(CString name);
	//根据Id判断是文件还是目录
	bool isFileById(int thisid,string thisname);
	//自定义消息对话框
	char* UTF8ToUnicode(const char* strSrc);
	int customMessagebox(CString message,DWORD dwStyle);
	int customMessagebox(string name,CString message,DWORD dwStyle);
	int customMessagebox(const vector<string>& name,const vector<CString>& message,DWORD dwStyle);
	int customMessagebox(CString caption,CString message,DWORD dwStyle);
	//文件大小单位
	CString fileSizeUnit(int filesize);
	//文件状态图标选择
	int fileLocalStatus(const std::string& strpath,const std::string& fileMD5);
	//根据路径字符串创建本地目录
	bool createLocalFolder(const std::string& folderPath);
	//文件目录，递归下载
	void recursiveDownloadDir(int thisid,int proJid,vector<string>&thisDirpath);
	//文件或文件夹下载到本地（type：TRUE-file,FALSE-folder）
	void customDownload(bool type,int thisid,int proJid,string downloadpath);
	void  timeFormatter(CString &time);
	char*  string2LPCTSTR(string sFrom);
}

enum FolderListcolumn{
	listcheck=0,  
	listicon,
	listname ,
	listtype,
	listsize,
	listauthor ,
	listcreatetime ,
	listversion,
	listrevision ,
	listlife ,
	listout ,
	listouttime ,
	listin ,
	listintime ,
	listnote ,	
};
enum HAPYicon{
	unknow=0,
	Wfolder,
	Yfolder,
	empty,
	checkoutBySelf,
	checkoutBySomeOne,
	filecheckin,
	localfilenotmatch,
	fileArchived,
	zip ,
	ipt ,
	doc ,
	docx ,
	vsd ,
	txt ,
	ses ,
	sldprt ,
	sldasm ,
	rvt ,
	rvm ,
	rtf ,
	rfa ,
	prt ,
	ppt ,
	pptx ,
	png ,
	one ,
	pdf ,
	nwf ,
	nwc ,
	mpp ,
	jpg ,
	ipj ,
	gif ,
	xlsx ,
	xls ,
	dxf ,
	dwg ,
	dgn ,
	csv ,
	catproduct ,
	catpart ,
	assembly ,
	myasm,
	iam ,
	idw ,
	slddrw ,
	ipn,
	thisend,
	/*记得改 MainFram->hIcon[47];*/
};