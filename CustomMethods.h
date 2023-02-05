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
	//������Ŀ��
	void createProjectTree(int projectid,CHAPYTreeCtrl* Tree,CBCGPBreadcrumb* breadcrumb);
	//������ݵ���Ŀ��Ŀ¼��·����
	void addDataToProjectTreeAndBreadcrumb(ProjectDir* dir, st_exec_errors& error,HTREEITEM treeChild,HBREADCRUMBITEM breadhChild,CHAPYTreeCtrl* Tree,CBCGPBreadcrumb* breadcrumb);
	//���Ŀ¼�������ļ���
	void addDirsToAttachedFileTree(vector<AttachmentFile*> AttachmentFiles,CBCGPGridRow* pchild,CHAPYTreeCtrl* tree); 
	//������ݵ������б�
	void addDataToReferenceList( int thisid,CHAPYTreeCtrl* tree,CBCGPComboBox* combo,int combonum);

	//List------------------------------------------------------------------------------------------------------
	//�����ļ���Id�����Ŀ¼
	void addSubDirsToFolderList(int folderId, ProjectDir* dir,CHAPYListCtrl* List);
	//�����ļ���Id����ļ�
	void addFilesToFolderList(int thisid,int projectid,CHAPYListCtrl* List);
	//��Ӱ汾�ļ�
	void addDataToVersionList(int thisid,CHAPYListCtrl* List);
	//����ļ��������ļ��б�
	void addFilesToAttachedFileList(int thisid,int projectid,CListCtrl& List,std::string thisname);
	//������ݵ��ļ������б�
	void addDataToFilePropertiesList(CBCGPPropList* m_propertylist,CHAPYListCtrl* List);
	//FolderList����
	void updateFolderList(int thisid,int projectid,CHAPYListCtrl* List);

	//����FileDetailsLayoutView
	void updateFileDetailsByTabNum(int tabnum,int thisid);
	//��Ŀ¼ѡ�񴰿�
	std::string openDirSelectDlg();
	//��·������ȡ�ļ���
	CString getFileNameByPath(CString strPath);
	//���ļ�������ȡ��׺��
	string getFileSuffixByFileName(CString name);
	//����Id�ж����ļ�����Ŀ¼
	bool isFileById(int thisid,string thisname);
	//�Զ�����Ϣ�Ի���
	char* UTF8ToUnicode(const char* strSrc);
	int customMessagebox(CString message,DWORD dwStyle);
	int customMessagebox(string name,CString message,DWORD dwStyle);
	int customMessagebox(const vector<string>& name,const vector<CString>& message,DWORD dwStyle);
	int customMessagebox(CString caption,CString message,DWORD dwStyle);
	//�ļ���С��λ
	CString fileSizeUnit(int filesize);
	//�ļ�״̬ͼ��ѡ��
	int fileLocalStatus(const std::string& strpath,const std::string& fileMD5);
	//����·���ַ�����������Ŀ¼
	bool createLocalFolder(const std::string& folderPath);
	//�ļ�Ŀ¼���ݹ�����
	void recursiveDownloadDir(int thisid,int proJid,vector<string>&thisDirpath);
	//�ļ����ļ������ص����أ�type��TRUE-file,FALSE-folder��
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
	/*�ǵø� MainFram->hIcon[47];*/
};