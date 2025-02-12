﻿#pragma once
#include "inrole.h"
#include "sfc_msg.h"
#include "PyWrapperUtils.h"
#include <QMap>
#include <QDateTime>
#include <QDataStream>
#include <QFileInfo>
#include <QPoint>
#include <QVector>
#include "JsonMessageUtils.h"
#include <QMapNode>
#include <QSet>

namespace INS {

	//项目文件类
	//*********************************************************************************************************
	struct ProjectRootFolderVO {
		qint32 projectId = 0;		//项目文件id
		QString projectName = "";	//项目名
		qint32 rootFolderId = -1;	//根文件夹id

		bool operator == (const ProjectRootFolderVO& prj_file) const {
			bool b_state(true);
			if(projectId != prj_file.projectId)
				b_state = false;
			if(projectName != prj_file.projectName)
				b_state = false;
			if(rootFolderId != prj_file.rootFolderId)
				b_state = false;

			return b_state;
		}

		bool operator != (const ProjectRootFolderVO& prj_file) const {
			bool b_state(true);
			if(*this == prj_file)
				b_state = false;

			return b_state;
		}

        Serialization(projectId, projectName, rootFolderId)
	};


	/*!
	 * \struct FolderSearchParam
	 * \ingroup file
	 * \brief 获取文件夹的参数
	 */
	struct FolderSearchParam {

		qint32 folderId = 0;//若folderId不为0,表示根据文件夹id来获取文件夹
		qint32 projectId = 0;//若项目id不为0,表示根据项目id来获取文件夹
		QString path = "";//路径查询参数

		Serialization(folderId, projectId, path)
	};

    //文件历史信息
    //*********************************************************************************************************
    struct FileVersionVO {
        qint32 fileId = -1;					//文件id
        qint32 projectId = 0;				//文件所属的项目
        qint32 version = 0;					//文件版本0
        qint32 authorId = 0;				//文件作者的id值
        QString authorName = "";			//作者名字
        IdName lastModifyBy;                //谁上传的这个文件
        QString comment = "";				//对文件最新上传时的评论
        QDateTime lastModifyTime = QDateTime();         //文件最后一次修改时间
        qint32 action = 1;					//产生该版本的动作
        qint64 size = 0;					//文件大小，字节
        QString checkCode;//校验码

    Serialization(fileId, projectId, version, authorId, authorName, lastModifyBy, comment, lastModifyTime, action, size, checkCode)
    };
    //*********************************************************************************************************

	//文件类的基类
	//**********************************************************************************************************************************************
	struct FileVO
	{
		qint32 fileId = 0;             //文件id0
		QString name = "";              //文件名（包含后缀)
		qint32 projectId = 0;           //文件所属的项目
		qint32 folderId = 0;            //所在文件夹id
		QString directory = "";         //文件相对于项目的路径,"/proj/xxx/yyy"

		QString checkCode = "";         //文件的md5值-1QString md5 = "";
		qint64 size = 0;                //文件大小，字节(上传需要)
		qint32 previewId = 0;           //预览文件id-1

		QDateTime createTime = QDateTime();         //文件创建时间
		QDateTime lastModifyTime = QDateTime();     //文件最后一次修改时间(上传需要)

		qint32 creatorId = 0;            //文件作者的id值
		QString creatorName = "";        //作者名字

		qint32 currentVersion = 0;                //文件版本0
		QMap<qint32, FileVersionVO> fileVersions;    //文件版本列表，已排好序

		QString comment = "";            //对文件最新上传时的评论
		qint32 status = 0;               //1=check in,2=check out.
		qint32 type = 0;                 //文件的类型 -1=empty folder
		QPair<qint32, QString> actionBy;    //最后check out这个文件的人0

		QList<IdName> tags;     //标签

		QDateTime checkoutTime;//checkout的时间
		QPair<qint32, QString> lastModifiedBy;//文件最后的修改者
		QPair<qint32, QString> checkoutBy;//被谁checkout的
		qint32 isIntermediate = 0; //是否为中间文件 0否 1是
		qint32 isReview = 0;  //是否需要审批 0否 1是

		Serialization(fileId, name, projectId, folderId, directory, checkCode, size, previewId, createTime,
			lastModifyTime, creatorId, creatorName, currentVersion, fileVersions, comment, status,
			actionBy, type, tags, lastModifiedBy, isIntermediate, isReview)
	};

    //文件夹类的基类
    //***********************************************************************************************************************************************
    struct FolderVO {
        qint32 folderId = -1;    //文件夹id
        qint32 projectId = 0;   //项目id
        QString directory = ""; //文件夹相对路径
        QString folderName = "";//文件夹名
        qint32 parentId = 0;    //父文件夹id,若为根，parent为0
        QMap<qint32, QString> folders;  //子文件夹id
        QMap<qint32, FileVO> files;     //子文件的信息
        QList<IdName> tags;     //标签信息

        Serialization(folderId, projectId, directory, folderName, parentId, folders, files, tags)
    };

	/*******************************************************************
	Description:用户信息
	********************************************************************/
	struct Person {
		qint32 id = 0;           //用户系统id
		QString account = "";          //用户名
        QString legal_name = "";		//名字 必填
        IdName gender;			//性别。---> IdName
        IdName job_title;			//职位 ---> IdName
        IdName status;			//工作状态 ---> IdName
        QString phone = "";				//电话号码
        QDate birthday;					//生日

        QList<IdName> subordinates;  //下属
        IdName superior;      //直属上司
        IdName team;          //所在团队
        IdName department; //所在项目组

		qint32 avatar = 0;           //头像文件id
		qint32 sysRoleId = (int)SystemRoleEnum::DEPARTMENT_GENERAL; //用户权限
		QString version = "";          //版本号
		bool sendNotification = false;

		Serialization(id, account, legal_name, gender, job_title, status, phone, birthday,
                      subordinates, superior, team, department, avatar, sysRoleId, version, sendNotification)
	};

	/*******************************************************************
	Description:团队类
	********************************************************************/
	struct Team {
		qint32 id = 0;					//团队的id，唯一标志。
		IdName department;		//部门
		QString name = "";				//团队名
		IdName  leader;			//队长
		QList<IdName> crews;		//用户名列表
		bool isdefault;					//是否为项目组的默认团队标记。
		QString version = "";			//版本号

        Serialization(id, department, name, leader, crews, isdefault, version)
	};

	/*******************************************************************
	Description:团队所在项目组信息
	********************************************************************/
	struct Department {
		qint32 id = 0;              //项目组的系统id
		QString name = "";          //项目组名
		QList<qint32> producers;    //项目组监制
		QList<qint32> directors;    //项目组导演
		QList<IdName> teams;   //项目组中的团队
		bool isdefault;             //是否为系统默认项目组标志。
		QString version = "";       //数据版本号
		IdName departmentRoot;
		QList<IdName> departmentAdmins;

        Serialization(id, name, producers, directors, teams, isdefault, version, departmentRoot, departmentAdmins)
	};

	//INFileStatusEnum
	//************************************************************************************************************************************************
	enum INFileStatusEnum {
		TO_DO = 1,				//即将要做的文件，一般为place holder。
		WIP = 2,				//work in process,正在制作中的文件。
		AWAITING_APPROVAL = 3,	//等待审核的文件。
		PASSED = 4,				//通过的文件。
		LOCKED = 5				//锁定的文件，不可编辑。
	};
	//*********************************************************************************************************************************************

	//回收站数据
	//*********************************************************************************************************
	struct INRecycleBinInfo {
		qint32 id = 0;						//key唯一索引
		qint32 iteid = 0;					//文件或文件夹id
		bool b_file = true;					//是否为文件，true表示文件，false表示文件夹
		QString name = "";					//文件或者是文件夹的名字，
		QString original_location = "";		//原始路径
		QDateTime date_deleted = QDateTime();	//删除日期时间
		QDateTime date_modified = QDateTime();//最后修改日期
		qint64 size = 0;						//文件或者文件夹大小，文件夹无大小。
		QString deleted_by = "";				//删除该文件夹或文件的用户姓名
	};
	//*********************************************************************************************************

	//服务端回复给客户端的回复信息
	//**************************************************************************************************
	class MessageInfo {
	public:
		qint32 code{ 0 };//成功或者失败
		QString message = {"unknown error"};//回复的字符串信息

		MessageInfo(){}

		MessageInfo(qint32 n_code)
			:code(n_code)
		{}

		MessageInfo(qint32 n_code, const QString& str_message)
			:code(n_code), message(str_message)
		{}

		bool isFail() {
			return code == 0;
		}

		static MessageInfo success() {
			return MessageInfo(1, "succeeded");
		}

		static MessageInfo fail() {
			return MessageInfo(0, "failed");
		}

		MessageInfo& operator=(const MessageInfo& other)
        {
			code = other.code;
			message = other.message;
			if (other.message.isEmpty())
				message =MessageInfo().message;
			return *this;
        }

		MessageInfo(const MessageInfo& other)
		{
			*this = other;
		}

        Serialization(code, message)
	};
	//**************************************************************************************************

	//**************************************************************************************************

	//人员性别的枚举
	//**************************************************************************************************
	class INGenderEnum {
	private:
		INGenderEnum() = delete;
		INGenderEnum(const INGenderEnum&) = delete;
		INGenderEnum& operator=(const INGenderEnum&) = delete;
	public:
		const static int MAILE = 0;//男
		const static int FEMALE = 1;//女
		static QString getValue(int key) {
			if(key != MAILE && key != FEMALE)
				return{};

			const std::map<int, QString> enumMap = {
				{ MAILE,"male" },
				{ FEMALE,"female" }
			};
			return enumMap.find(key)->second;
		}
	};
	//**************************************************************************************************

	//用工状态的枚举
	//**************************************************************************************************
	class INStatusEnum {
	private:
		INStatusEnum() = delete;
		INStatusEnum(const INStatusEnum&) = delete;
		INStatusEnum& operator=(const INStatusEnum&) = delete;
		const static int minKey = 0;
		const static int maxKey = 4;
	public:
		const static int FULLTIME = 0;//全职
		const static int FIRED = 1;//解雇
		const static int RESIGNED = 2;//辞职
		const static int ONLEAVE = 3;//休假
		const static int PARTTIME = 4;//兼职

		static QString getValue(int key) {
			if(key < minKey || key > maxKey)
				return "";
			const std::map<int, QString> enumMap = {
				{ FULLTIME,"Full_time" },
				{ FIRED,"Fired" },
				{ RESIGNED,"Resigned" },
				{ ONLEAVE,"On_leave" },
				{ PARTTIME,"Part_time" }
			};
			return enumMap.find(key)->second;
		}
	};

	struct INPipelineStep;
	//项目的数据
	struct ProjectData {
		qint32 assetCount = 0;//项目资产数量
		qint32 shotCount = 0;//项目镜头数量
		qint32 taskCount = 0;//任务的数量
		QMap<qint32, qint32> assetData;//资产数据
		QMap<qint32, qint32> shotData;//镜头数据

        Serialization(assetCount, shotCount, taskCount, assetData, shotData)
	};


	struct INProjectBaseObj {
		qint32 projectId = 0;						//项目Id
		QString projectName = "";						//项目名称
		QDateTime issueDate = QDateTime();			//计划开始日期
		QDateTime dueDate = QDateTime();			//计划结束日期
		QDateTime actual_start_time = QDateTime();	//实	际开始日期
		QDateTime actual_end_time = QDateTime();	//实	际结束日期
		QString description = "";					//描	述
		QString color = "";							//颜	色
		qint32 status = 0;							//状	态
        bool workfloweditflag = false;
        QPair<qint32,QString> defaultApprover;               //默认审批员ID

        //注意！！！更改这个类，要将对应的子类加上序列化成员，否则，新增、减的成员在子类无效。
#ifndef IN_PROJECT_BASE_PROPERTY
#define IN_PROJECT_BASE_PROPERTY projectId,projectName,issueDate,dueDate,actual_start_time,actual_end_time,\
    description,color,status,workfloweditflag,defaultApprover
#endif

        Serialization(IN_PROJECT_BASE_PROPERTY)
	};

	//项目的前端页面数据
	struct INQProjectVO :public INProjectBaseObj {
		QSet<QString> fileServerRoot;//文件服务器的服务器根。

		IdName projectRoot;//项目存放的项目根管理者id和名称的键值对
		IdName department;//项目存放的项目根管理者id和名称的键值对

		ProjectData projectData;//项目统计数据
        QList<IdName> tags;
        qint32 type = 0;        //项目类型, film:1,tv:2
        QString rootDir;        //项目映射根路径

        Serialization(IN_PROJECT_BASE_PROPERTY ,fileServerRoot, projectRoot, department,
                      projectData, tags, type, rootDir)
	};

    //获取项目业务的参数过滤类
    struct INQGetPrjBusinessFilter {
        enum FetchEnum {
            Basic = 1,
            ObjectName = 2,
            StatisticsData = 3,
            Tag = 4,
            SeasonAndEpisode = 5
        };

        QSet<qint32> fetchData;
        QSet<qint32> projectIds;
        QDateTime startTime;
        QDateTime dueTime;
        bool isNeededDetails = false;
        qint32 teamId = 0;//团队id
        QString projectName = "";
		qint32 isDeleted = 0; //-1 全部，0 未删除，1 已经删除 默认查询未删除的项目

        Serialization(fetchData, projectIds, startTime, dueTime, isNeededDetails, teamId, projectName, isDeleted)
    };

    struct PipeLineStepApproval
    {
        qint32  pstApprovalId           = 0;    //审批流程ID
        qint32  projectId               = 0;    //项目ID
        qint32  pipelineId              = 0;    //pipeline step表主键
        qint32  personId                = 0;    //审批员ID
        QString name                    = "";   //审批员姓名
        qint32  avatar                  = 0;    //审批员头像ID
        qint32  parentPstApprovalId     = 0;    //父节点审批流程ID
        qint32 operation                = 0;    //用户操作，-1 删除；0 默认；1 新增
        QDateTime   createTime;
        QDateTime   updateTime;

        Serialization(pstApprovalId,projectId, pipelineId, personId, name,avatar,parentPstApprovalId,operation,createTime,updateTime);
    };

	struct INQWorkPath {
		qint32 pathId = 0;
		qint32 pipelineId = 0;
		QString filePath = "";
		qint32 fileType = 0;
		qint32 fileID = 0;
        QString fileName = "";
        qint32 isIntermediate = 0; /// 是否为中间文件 0否 1是
        qint32 isReview = 0; /// 是否为审批文件 0否 1是

        Serialization(pathId, pipelineId, filePath, fileType, fileID, fileName,isIntermediate,isReview);
	};

	struct INPipelineStep {

		qint32 pipelineId = 0;      //pipeline的id
		qint32 pipelineType = 0;    //pipeline的类型
		QString pipelineName = "";  //pipeline的名称
		qint32 projectId = 0;       //项目id
		QPair<qint32, QString> coordinator;//由谁分配任务
		QPair<qint32, QString> supervisor; //审核人id,姓名
		QString description = "";
        QList<INQWorkPath> outputFileList;//输出文件列表
        QList<PipeLineStepApproval> approvalList;//审批员列表
        IdName creator;//pipeline step 的创建者
        bool toValidate = false;//require technical validation
        QString loaderScript = "";
        QString validationScript = "";

        Serialization(pipelineId, pipelineType, pipelineName, projectId, coordinator,
                supervisor, description, outputFileList, approvalList, creator,toValidate,loaderScript,validationScript);
	};

	//struct INQWorkPath {
	//	qint32 pathId = 0; //主键
	//	qint32 pipelineId = 0; //pstId
	//	QString filePath = "";//路径
	//	qint32 fileType = 0; //文件类型 0 文件夹 1文件
	//	qint32 fileID = 0; //文件id
	//	QString fileName = "";//文件名
	//	qint32 isIntermediate = 0; //是否为中间文件 0否 1是
	//	qint32 isReview = 0; //是否为审批文件 0否 1是

	//	Serialization(pathId, pipelineId, filePath, fileType, fileID, fileName, isIntermediate, isReview)
	//};

    //struct PipeLineStepApproval
    //{
    //    qint32  pstApprovalId           = 0;    //审批流程ID
    //    qint32  projectId               = 0;    //项目ID
    //    qint32  pipelineId              = 0;    //pipeline step表主键
    //    qint32  personId                = 0;    //审批员ID
    //    QString name                    = "";   //审批员姓名
    //    qint32  avatar                  = 0;    //审批员头像ID
    //    qint32  parentPstApprovalId     = 0;    //父节点审批流程ID
    //    qint32 operation                = 0;    //用户操作，-1 删除；0 默认；1 新增
    //    QDateTime   createTime;
    //    QDateTime   updateTime;

    //    Serialization(pstApprovalId,projectId, pipelineId, personId, name,avatar,parentPstApprovalId,operation,createTime,updateTime);
    //};

	//work-flow详情
	struct WorkFlowDetlFile;
	struct INQWorkFlowDetail {
		qint32 workFlowStepId = 0;      //工作流每一步的id
		QString workFlowStepName = "";  //工作流每一步的名称
		QString alias;
		qint32 workFlowId = 0;          //工作流模板的id
		QList<qint32> parentStepIds;    //工作流父亲节点的id集合
		QString color = "#FFFFFF";      //描述颜色
		QPair<qint32, QString> pipelineStep;
		QPair<QPoint, QPoint> position;
		qint32 wftype = 0;
		qint32 containerId = 0;
		QList<WorkFlowDetlFile> wfdFileList;

		Serialization(workFlowStepId, workFlowStepName, alias, workFlowId,
					  parentStepIds, color, pipelineStep, position, wftype, containerId, wfdFileList)
	};

	//work-flow
	struct INQWorkFlowVO {

		qint32 workFlowId = 0;//工作流模板的id
		QString workFlowName = "";//工作流名称
		qint32 projectId = 0;//所属项目的id
		qint32 pipelineType = 1;//所属pipeline的类型,1为Asset,2为Shot'
		QList<INQWorkFlowDetail> workFlowDetails;
        IdName creator;

		//对象序列化
        Serialization(workFlowId, workFlowName, projectId, pipelineType, workFlowDetails, creator)
	};

	struct LogInfoVO {
		qint32 logId = 0;//日志的id
		qint32 logTypeId = 0;//业务类型
		QString logDetail = "";//日志详情
		QDateTime createTime;//日志产生时间
		QString clientIp = "";//客户端IP地址
		bool success = false;//客户端请求是否成功

		QPair<qint32, QString> role;//用户角色
		QPair<qint32, QString> user;//用户
		QPair<qint32, QString> action;//用户操作
		//对象序列化

        Serialization(logId, logTypeId, logDetail, createTime, clientIp, role, success, user, action)
	};

	struct LogBusParams {
		qint32 logTypeId = 0;
		QDateTime fromTime;
		QDateTime toTime;

        Serialization(logTypeId, fromTime, toTime)
	};

	//任务过滤条件
	//**************************************************************************************************
	struct INTaskSeachCondition {
		int projectId = 0;
		int teamId = 0;
		QDateTime beginTime = QDateTime();
		QDateTime endTime = QDateTime();
		bool unassigned = false; //筛选未分配的task

        Serialization(projectId, teamId, beginTime, endTime, unassigned)
	};

	//任务的结构体
	struct INTaskBaseObj {
		qint32 projectId = 0;          //项目Id
		QString project;               //项目名称
		QDateTime issueDate;           //计划开始日期
		QDateTime dueDate;             //计划结束日期
		QDateTime actual_start_time;   //实际开始日期
		QDateTime actual_end_time;     //实际结束日期
		QDateTime privilege_start_time;  //特权开始日期
		QDateTime privilege_end_time;  //特权结束日期
		QDateTime createdTime;			//创建时间
		QString description;           //描述
		QString color;                 //颜色
		QString pos;					//位置
		qint32 status = 0;             //状态
	};

    ///任务审批结构体
    struct TaskApproval
    {
        qint32  taskApprovalId          = 0;    //审批流程ID
        qint32  projectId               = 0;    //项目ID
        qint32  taskId                  = 0;    //任务id
        qint32  personId                = 0;    //审批员ID
        QString name                    = "";   //审批员姓名
        qint32  avatar                  = 0;    //审批员头像ID
        qint32  parentTaskApprovalId    = 0;    //父节点审批流程ID
        qint32  isCurrent               = 0;    //任务当前审批人 0 否；1 是
        qint32  type                    = 0;    // 0 普通；1加急
        qint32  status                  = 0;    // 0 未审批；1已审批；2 send to previous; 3 reopen
        qint32  operation               = 0;    //用户操作, -1 删除；0 默认；1 新增
        QDateTime   createTime;
        QDateTime   updateTime;

        Serialization(taskApprovalId,projectId, taskId, personId, name,avatar,
                      parentTaskApprovalId,isCurrent,type,status,operation,createTime,updateTime)
    };

	//人员的任务的结构体
	//**************************************************************************************************
    struct INTask : public INTaskBaseObj {
		qint32 taskId{ 0 };					//任务id
		qint32 objectId{ 0 };				//Asset/shotid
		QString objectName;					//Asset/shot名称
		QString objectAlias;                //asset/shot的别名
		QString taskName;				    //任务名
		QString alias;                      //别名
		qint32 assigneeTeamId{ 0 };			//任务执行人所属组id
		QString assigneeTeam;			    //任务执行人所属组
		qint32 assigneePersonId{ 0 };		//任务执行人id
		QString assigneePerson;			    //任务执行人
		qint32 pipelineStepId{ 0 };			//pipelinestepid
		QString pipelineStep;			    //pipelinestep名称
		qint32 supervisorId{ 0 };		    //任务审批人id
		QString supervisor;				    //任务审批人名
		qint32 coordinatorId{ 0 };          //分派者id
		QString coordinator;                //分派者
		qint32 organizerId{ 0 };			//项目创建人id
		QString organizer;				    //项目创建人
		qint32 reminderId{ 0 };
		qint32 difficulty{ 0 };			    //难度
		qint32 type{ 0 };				    //0:Asset/1:Shot
		qint32 wftype{ 0 };						//0节点，1容器
		qint32 workFlowStepId{ 0 };				//wf_step_id
		qint32 containerId{ 0 };                 //对应wftype为1的taskId
        QString storageType;				//存储类型
        qint32 taskApprovalId{0};           //当前审批列表ID
        QList<TaskApproval> approvalList;   //审批员列表，用于查看。
        QList<IdName> tags;
        IdName scene;

		qint32 toValidate = 0;//是否需要科技验证
		QString loaderScript = "";
		QString validationScript = "";

		QDateTime lastModifyTime;

		//taskId,projectId,containerName,taskName,pipelineStep,supervisor,organizer,color,status,type不能为空
		bool checkTaskParam() {
			if(projectId == 0 || objectId == 0 || taskName.isEmpty() || organizerId == 0
			   || color.isEmpty() || status != 0 || type > 1 || type < 0)
				return false;
			else
				return true;
		}

		Serialization(projectId, project, issueDate, dueDate, actual_start_time, actual_end_time, privilege_start_time, privilege_end_time, createdTime,
					  description, color, pos, status, taskId, objectId, objectName, taskName, alias, assigneeTeamId, assigneeTeam,
					  assigneePersonId, assigneePerson, pipelineStepId, pipelineStep, supervisorId, supervisor,
					  coordinatorId, coordinator, organizerId, organizer, reminderId, difficulty, type, wftype, workFlowStepId,
                      containerId, storageType,taskApprovalId,approvalList, tags, toValidate,
					  loaderScript, validationScript, lastModifyTime)
	};

    //
    struct TaskApprovalWorkFlow
    {
        qint32 projectId = 0; // 项目ID
        qint32 taskId = 0; // 任务ID
        QList<qint32> personList; // 审批人员ID列表

        Serialization(projectId, taskId, personList);
    };

    struct TaskRelationFile {
        qint32 fileId           = 0;  //文件ID。必填
        qint32 variantId        = 0;  //variantId。requiredFile必填，其他可填。
        qint32 isIntermediate   = 0;  //
        qint32 isReview         = 0;  //
        QString path            = ""; //文件所在的目录，不包括文件名。以"/"开头，但不以"/"结尾。例如：/ab/cd/ef
        QString name            = ""; //文件名,包括后缀。例如：abc.txt

    Serialization(fileId,variantId,isIntermediate, isReview, path, name);
    };
    struct TaskRelationFileRequest {
        qint32 taskId           = 0; //任务ID。 必填
        qint32 fileType         = 0; //文件类型。0 referenceFile;1 repositoryFile(废弃);2 requiredFile; 3 outputFile
        QList<TaskRelationFile> fileList;

    Serialization(taskId, fileType, fileList);
    };


	struct FileComment {
		QString comment;
		QString commentBy;
		QDateTime date;

		Serialization(comment, commentBy, date);
	};
	struct OutputFileRequest {
		qint32  projectId = 0;    //项目ID  [必填]
		qint32  objectType = 0;    //类型  0 资产 ; 1 镜头  [必填]
		qint32  pipelineStepId = 0;    //工作节点ID [必填]
		QString assetName = "";   //资产名、资产别名  需要支持模糊匹配 [可选]
		qint32  seasonId = 0;    //季度ID TV项目[必填]，FILM项目[可选]
		qint32  episodeId = 0;    //集数ID [可选]
		qint32  sceneId = 0;    //场次ID [可选]
		qint32  shotNumber = 0;    //镜头号  需要支持模糊匹配 [可选]
		QString fileName = "";   //文件名  需要支持模糊匹配 [可选]

		Serialization(projectId, objectType, pipelineStepId, assetName, seasonId, episodeId, sceneId, shotNumber, fileName)
	};

	struct TaskTree {
		qint32  taskId = 0;        //任务ID
		QString taskName = "";       //任务名称
		QString alias = "";       //任务别名
		qint32  pipelineStepId = 0;        //pstId
		QString pipelineStepName = "";       //pst节点名称
		qint32  status = 0;        //任务状态 None = 0,ToDo = 1,WorkInProgress = 2,PendingApproval = 3,PendingPanelReview = 4,Reopen = 5,PendingValidation = 6,Approval = 7,Paused = 8,Deprecated = 9,OnHold = 10
		QList<FileVO> fileList;                 //任务输出文件列表

		Serialization(taskId, taskName, alias, pipelineStepId, pipelineStepName, status, fileList)

			bool operator==(const TaskTree& task) const {
			return task.taskId == this->taskId;
		}
	};
	struct AssetVariant {
		qint32  variantId = 0;            //variantId
		QString variantName = "";           //名称
		QList<TaskTree> taskList;               //任务列表

		Serialization(variantId, variantName, taskList)

			bool operator==(const AssetVariant& av) const {
			return av.variantId == this->variantId;
		}
	};

	struct ObjectAsset {
		qint32  assetId = 0;            //资产ID
		QString assetName = "";           //资产名称
		QString alias = "";           //资产别名
		QList<AssetVariant>     variantList;    //资产下variant列表

		Serialization(assetId, assetName, alias, variantList)

			bool operator==(const ObjectAsset& oa) const {
			return oa.assetId == this->assetId;
		}
	};
	struct ObjectShot {
		qint32  shotId = 0;            //镜头ID
		QString shotName = "";           //镜头号
		QList<TaskTree> taskList;               //任务列表

		Serialization(shotId, shotName, taskList)
	};

	struct ObjectScene {
		qint32  sceneId = 0;
		QString sceneName = "";
		QList<ObjectShot>   shotList;

		Serialization(sceneId, sceneName, shotList)
	};
	struct ObjectEpisode {
		qint32  episodeId = 0;
		QString episodeName = "";
		QList<ObjectScene>  sceneList;

		Serialization(episodeId, episodeName, sceneList)
	};
	struct ObjectSeason {
		qint32  seasonId = 0;
		QString seasonName = "";
		QList<ObjectEpisode> episodeList;

		Serialization(seasonId, seasonName, episodeList)
	};
	struct OutputFileResponse {
		qint32  projectId = 0;    //项目ID
		QString projectName = "";   //项目名称
		qint32  projectType = 0;    //项目类型 1 电影; 2 电视
		qint32  objectType = 0;    //类型  0 资产 ; 1 镜头
		QList<ObjectAsset>    assetList;  //资产列表
		QList<ObjectSeason>   seasonList; //镜头列表

		Serialization(projectId, projectName, projectType, objectType, assetList, seasonList)
	};


	//**************************************************************************************************

	////////// 文件相关 /////////////////////////
	/*!
	 * \struct FileTransferInfo
	 * \ingroup
	 * \brief 文件的传输信息
	 */
    struct FileTransferInfo {
        //文件对应的存储服务器的信息
        QString storageId = "";//文件服务器的文件id，若是上传,该字段信息为空

        QString token = "";//文件传输的校验码
        QString path = "";//文件服务器的文件路径
        QString name = "";//文件在存储服务器的文件名称,一般跟应用服务器的文件名不相同
        qint64 size = 0;//文件大小
        QString ip = ""; //存储服务器IP地址
        qint32 port = 0; //存储服务器的端口

        FileVO clientInfo;//文件的客户端信息
        qint32 transferRecordId = 0; //文件传输信息

        Serialization(storageId, token, path, name, ip, port, size, clientInfo, transferRecordId)
    };

	struct INTaskComment {
		qint32 commentId = 0;
        QPair<qint32, QString> person;
		qint32 taskId = 0;
		qint32 taskStateId = 0;
		QDateTime submitDate;
		QString text;
        QList<qint32> audioIds;
        QList<qint32> videoIds;
        QList<qint32> pixmapIds;
        qint32 isApprover = 0;  /// 提交此评论是否为任务审批者标志位 0 否 1 是
        qint32 taskStatus = 0;  /// 提交评论时，该任务的状态
        QDateTime statusDate;   /// 提交评论时，该任务的状态变更时间

        Serialization(commentId, person, taskId, taskStateId, submitDate, text, audioIds, videoIds,
                      pixmapIds,isApprover,taskStatus,statusDate)
	};

	//资产的结构体
	//**************************************************************************************************
	struct INAsset : public INProjectBaseObj {
		qint32 assetId = 0;					//资产id
		QString assetName = "";				//资产名
		QString alias;				//资产名
		qint32 workflowId = 0;
		QString workflow = "";
		qint32 uploadFootageId = 0;
		QList<INTask> createtasklist;	//任务列表
        IdName creator;
        QList<IdName> tags;             //标签列表

        Serialization(IN_PROJECT_BASE_PROPERTY, assetId, assetName, alias, workflowId, workflow, uploadFootageId,\
            createtasklist, creator, tags)
	};
	//**************************************************************************************************

	//镜头的结构体
	//**************************************************************************************************
	struct INShot : public INProjectBaseObj
	{
        qint32 shotId{0};		    //镜头id
        qint32 seasonId{0};        //季 ID
        QString seasonName{""};     //季名称
        qint32 episodeId{0};       //剧集ID
        QString episodeName{""};    //剧集名称
        qint32 sceneId{0};		    //场次id
        QString scene{""};          //场次名
        QString shotName{""};       //镜头名
        qint32 workflowId{0};
        QString workflow{""};
        qint32 uploadFootageId{0};
		QList<INTask> createtasklist;	//任务列表
		IdName creator;                 //创建者id和名字
		QList<IdName> tags;
        qint32 projectType{0};             //镜头所属的项目类型。1： film，2: tv

		Serialization(IN_PROJECT_BASE_PROPERTY, shotId, seasonId, seasonName, episodeId, episodeName, sceneId, scene,
			shotName, workflowId, workflow, uploadFootageId, createtasklist, creator, tags, projectType)
	};
	//**************************************************************************************************

	//INSequence
    //**************************************************************************************************
    struct INSequence : public INProjectBaseObj
    {
        qint32 sequenceId       = 0;    //主键id
        qint32 seasonId         = 0;    //季 ID
        QString seasonName      = "";   //季名称
        qint32 episodeId        = 0;    //剧集ID
        QString episodeName     = "";   //剧集名称
        qint32 sceneId          = 0;    //场次id
        QString sceneName       = "";   //场次名
        qint32 workflowId       = 0;
        QString workflowName    = "";
        QList<INTask> taskList;	        //任务列表
        IdName creator;                 //创建者id和名字
        QList<IdName> tags;
        qint32 projectType  = 0;        //镜头所属的项目类型。1： film，2: tv

        Serialization(IN_PROJECT_BASE_PROPERTY, sequenceId, seasonId, seasonName,episodeId,episodeName,sceneId, sceneName,
                workflowId, workflowName, taskList, creator, tags, projectType)
    };

	//镜头文件
	struct INSequenceShotFile {
		qint32  shotGroupId     = 0;    //镜头组ID
		qint32  shotNum         = 0;    //镜头号码 从1开始递增
		qint32  fileId          = 0;    //文件ID
		qint32  fileVersion     = 0;    //文件版本号
        QString filePath        = "";   //文件路径
        QString fileName        = "";   //文件名
        QString checkCode       = "";   //文件校验码
		qint32  duration        = 0;    //影片时长，单位：帧
		qint32  sequenceNum     = 0;    //文件序列号 从1开始递增
		qint32  isSelected      = 0;    //是否使用 0 否  1是
		qint32  version         = 0;    //镜头组版本
		qint32  operation       = 0;    //0 默认；1 新增；2 删除；3 恢复
		QList<QString> freqContentList;  //视频标签数据列表

        Serialization(shotGroupId,shotNum,fileId,fileVersion,filePath,fileName,checkCode,duration,
				sequenceNum,isSelected,version,operation,freqContentList)
	};

	//镜头组审批数据
	struct INSequenceShotGroupReview {
		qint32 taskId           = 0;    //任务id
		qint32 shotGroupId      = 0;    //镜头组id
		qint32 version          = 0;    //镜头组版本
		qint32 fileId           = 0;    //文件id
		qint32 fileVersion      = 0;    //文件版本
		QString freqContent     = "";   //视频标签数据
		QString text            = "";   //review内容
        QSet<qint32> pixmapIds;        //评论图片文件Id集合

        Serialization(taskId,shotGroupId,version,fileId,fileVersion,freqContent,text,pixmapIds)
	};


	//镜头组
    struct INSequenceShotGroup {
        qint32  shotGroupId             = 0;    //镜头组ID
        qint32  projectId               = 0;    //*项目ID
        qint32  sequenceId              = 0;    //*sequenceId
        qint32  taskId                  = 0;    //*任务ID
        QString groupName               = "";   //*名称
        QString description             = "";   //描述
        qint32  version                 = 0;    //镜头组版本
        qint32  isDefault               = 0;    //是否为默认列表 0 否 1是
	    qint32  isApproved              = 0;    //是否审批通过 0否 1是
	    qint32  shotCount               = 0;    //镜头数量
        qint32  totalDuration           = 0;    //当前镜头总时长 单位：秒
        qint32  totalFreq               = 0;    //当前镜头总修改意见次数
        qint32  totalShotGroup          = 0;    //任务总镜头组数量
        IdName  creator;                        //创建者id和名字
        QDateTime   createTime;                 //创建时间
        QDateTime   updateTime;                 //更新时间
        QMap<qint32,QList<INSequenceShotFile>>  shotFileMap; //镜头列表 key 镜头号码 value 文件列表

        Serialization(shotGroupId, projectId, sequenceId, taskId,groupName,
                description,version,isDefault, isApproved,shotCount,totalDuration,totalFreq,totalShotGroup,
                creator,createTime, updateTime,shotFileMap)
    };

    //镜头文件回收站
    struct INSequenceShotFileRecycle {
        qint32 shotFileRecycleId    = 0;    //镜头组文件回收站id
        qint32 shotGroupId          = 0;    //镜头组表ID
        qint32 fileId               = 0;    //文件id
        qint32  fileVersion         = 0;    //文件版本号
        QString fileName            = "";   //文件名
        IdName  creator;                    //创建者id和名字
        QDateTime createTime;               //创建时间

        Serialization(shotFileRecycleId,shotGroupId,fileId,fileVersion,fileName,creator,createTime)
    };

    // 镜头组下可创建的镜头列表信息结构体
    struct INSequenceShotStatus {
        qint32 projectId        = 0;        //项目Id
        qint32 projectType      = 0;        //所属的项目类型。1： film，2: tv
        QString seasonName      = "";       //季名称
        QString episodeName     = "";       //剧集名称
        QString sceneName       = "";       //场次名称
        QMap<qint32,qint32> shotStatusMap;  //镜头列表 key 镜头号码 value 是否已经创建 0 未创建 1已经创建

        Serialization(projectId,projectType,seasonName,episodeName,sceneName,shotStatusMap)
    };

    //**************************************************************************************************


    /*!
	 * \brief The RequiredFileStruct struct Required文件描述结构体
	 */
	struct RequiredFileStruct {
		qint32 taskID = 0; /*!< 产出此文件的任务ID */
		QString taskName = ""; /*!< 产出此文件的任务名字 */
		qint32 taskStatus = 0; /*!< 产出此文件的任务的当前状态 */
		FileVO requiredFile; /*!< 文件结构体 */
	};

	/////////////////////////标签部分/////////////////////////////////////////////////
	//标签资源的视图对象
	struct TagInfoVO {
		//标签信息
		qint32 tagId = 0;//标签的id
		QString tagName;//标签的名称
		QString color;//标签颜色
		qint32 tagUsedCount = 0;//标签使用次数

		//序列化
		Serialization(tagId, tagName, color, tagUsedCount)
	};

	//标签资源的视图对象
	struct TagResourceVO {
		QList<TagInfoVO> tagInfos;//标签信息
		QPair<qint32, QString> tagGroup;//所属的标签组信息
		QPair<qint32, QString> tagObject;//标签对象信息
		QVariantMap tagResourceInfo;//具体的标签资源信息

		Serialization(tagInfos, tagGroup, tagObject, tagResourceInfo)
	};

	//标签参数
	struct TagParam {
		qint32 tagObjectId = 0;//标签对象id,标签类型[必须的]
		QString tagName = "";//标签的名称[必须的]
		qint32 resourceId = 0;//标签资源id[必须的]
		qint32 resourceEntryId = 0;//资源组root的id[非必须,默认为resourceId]
		qint32 tagGroupId = 0;  //Param searchParamTag
		QString color = "";//标签颜色

		Serialization(tagObjectId, tagName, resourceId, resourceEntryId, color)
	};

	struct SequenceFileParam {
		qint32 taskId = 0;
		qint32 type = 0;
		QString squenceName = "";
		QSet<QPair<qint32, qint32>> fileVersionInfos;

		Serialization(taskId, type, squenceName, fileVersionInfos)
	};

	/////////////////////////Work log部分/////////////////////////////////////////////////
	class SimpleWorklog {
	public:
		qint32 worklogId;
		qint32 publisherid;
		QDateTime publicationDate;
		QString content;

        Serialization(worklogId, publisherid, publicationDate, content)
	};

	//项目的里程碑
	struct ProjectMilestone
	{
		int milestoneId = 0;//里程碑id
		int projectId = 0;//项目id
		QDateTime timePoint;//时间点
		QString description = "";//描述
		QString color = "";//颜色
		QDateTime createTime;//创建时间
		QString milestoneName = "";
		Serialization(milestoneId, projectId, timePoint, description, color, createTime, milestoneName)
	};

	enum class NotificationHierarchy
	{
		UNKNOWN = 0,
		USR_ACTIVATED = 1,
		SYS_GENERATED = 2
	};

	enum class NotificationCategory
	{
		UNKNOWN = 0,
		PROJECT = 1,
		ASSET = 2,
		SHOT = 4,
		ACCOUNT_LOGIN = 8,
	};

	class NotificationRecipient
	{
	public:
		NotificationRecipient() = default;
		NotificationRecipient(qint32 userID, QString userName) :
			recipientID(userID), recipientName(userName) {}

	public:
		qint64 receiveID { 0 };

		qint32 recipientID { 0 };

		QString recipientName;

		bool received { false };

		Serialization(receiveID, recipientID, recipientName, received);
	};

	class Header
	{
	public:
		Header() = default;
		Header(qint32 userID, QString userName, NotificationHierarchy hierarchy, NotificationCategory category) :
			senderID(userID),
			senderName(userName)
		{
			this->hierarchy = (qint32)hierarchy;
			this->category = (qint32)category;
		}

	public:
		qint64 notificationID{ 0 }; // notification id

		qint32 senderID{ 0 }; // sender id

		QString senderName; // sender name

		qint32 hierarchy { (qint32)NotificationHierarchy::UNKNOWN };

		qint32 category { (qint32)NotificationCategory::UNKNOWN };

		QDateTime timestamp; // notification creation time

		QList<NotificationRecipient> recipients;

		Serialization(notificationID, senderID, senderName, hierarchy, category, timestamp, recipients);
	};

	class NotificationMessage
	{
	public:
		Header header;
		QString payload;

		Serialization(header, payload);
	};

	//// 分类的消息
	//class Recipient {
	//public:
	//	qint64 recv_id{ -1 };
	//	qint32 recipient_id{ -1 }; // recipient folderId
	//	QString recipient_name; // recipient name
	//	bool read{false }; // status

	//	Recipient() = default;

	//	Recipient(qint32 rid, const QString &rname) {
	//		recipient_id = rid;
	//		recipient_name = rname;
	//	}

 //       Serialization(recv_id, recipient_id, recipient_name, read);
	//};

	//class NotificationHeader {
	//public:
	//	qint64 notification_id{-1 }; // notification folderId
	//	qint32 sender_id{-1 }; // sender folderId
	//	QString sender_name; // sender name
	//	QDateTime timestamp; // notification creation
	//	qint32 type{ -1 };

	//	NotificationHeader() = default;
	//	NotificationHeader(qint32 sid, const QString& sname) : sender_id(sid), sender_name(sname) {}

	//	friend QDataStream& operator<<(QDataStream& out, const NotificationHeader& header) {
	//		out << header.notification_id << header.sender_id << header.sender_name << header.timestamp << header.type;
	//		return out;
	//	}

 //       Serialization(notification_id, sender_id, sender_name, timestamp, type);
	//};

	//class Notification {
	//public:
	//	NotificationHeader header;
	//	QString msg;
	//	QVector<Recipient> recipients;

 //       Serialization(header, msg, recipients);
	//};

	struct Avatar {
		qint32 personId;
		QByteArray picavatar;
		bool isdefault = false;
	};

	/*!
	 * \struct StorageInfo
	 * \ingroup file
	 * \brief 存储服务器的信息
	 */
	struct StorageInfo {
		QString storageType = ""; //存储服务器类型  HDD or SSD
		QString groupName = ""; //存储服务器组名 group1
		QString instanceId = ""; //存储实例ID
		QString ip = ""; //存储服务器IP
		qint32 port = 0; //端口
		qint64 totalSpace = 0; //总空间  单位:byte
		qint64 freeSpace = 0; //可用空间 单位:byte
		QString basePath = ""; //根目录

		Serialization(storageType, groupName, instanceId, ip, port, totalSpace, freeSpace, basePath)
	};

	/**
     * 筛选的额外条件
     */
	struct ExtraFilter {

		int fromStatus = 0;//from状态，当且仅当fileterFlag = 1时候有效
		int toStatus = 0;//to状态，当且仅当fileterFlag = 1时候有效

		int fileType = 0;//文件类型，当且仅当filterFlag = 1的时候有效

		int filterFlag = 0;//0: 表示无效， 1: 表示By Task Status 2: 表示By File Change

		QDateTime fromTime;
		QDateTime toTime;

		bool operator==(const ExtraFilter& other) {
			if(fromStatus != other.fromStatus)
				return false;
			if(toStatus != other.toStatus)
				return false;
			if(fileType != other.fileType)
				return false;
			if(filterFlag != other.filterFlag)
				return false;
			if(fromTime != other.fromTime)
				return false;
			if(toTime != other.toTime)
				return false;
			return true;
		}

		bool operator!=(const ExtraFilter& other) {
			return !(*this == other);
		}

		Serialization(fromStatus, toStatus, fileType, filterFlag, fromTime, toTime)
	};

	//任务过滤参数
	struct TaskFilterParam {
		enum FetchEnum {
			Basic = 1,
			ObjectName = 2,
			ApprovalList = 3,
			Tag = 4
		};
		QSet<qint32> fetchData;

		qint32 projectId = 0;//根据项目id查询
		QSet<qint32> objectIds;//根据项目的资产或者镜头的id查询
		QSet<qint32> types;//筛选资产类型或者镜头类型 0:资产,1:镜头
		QSet<qint32> status;//根据项目的状态查询

		qint32 assignTeamId = 0;//根据分配的团队查询
		QSet<qint32> assigneeIds;//根据分配的人查询
		QSet<qint32> supervisorIds;//根据supervisorId查询
		QSet<qint32> pipelineStepIds;//根据pipelineStepId查询

		QDateTime issueTime;//根据任务的发布时间查询
		QDateTime dueTime;//根据任务的截至时间查询
		bool isUnassigned = false; //筛选未分配的task

		QSet<qint32> taskIds;//根据taskId查询

		bool isPendingAppend = false;//是否是pandingAppend的任务

		QSet<QString> tagNames;//标签条件搜索

		QString alias;          //别名

		QString tagNameRegExp;//标签条件搜索

		qint32 excludeDeletedItem = 0;//是否排除已经删除的项 0:排除已经删除的 1:包含已经删除的 2:只包含已经删除的

		ExtraFilter extraFilter;//额外的刷选条件



		bool operator==(const TaskFilterParam& other)
		{
			if(fetchData != other.fetchData)
				return false;
			if (projectId != other.projectId)
				return false;
			if (types != other.types)
				return false;
			if(status != other.status)
				return false;
			if(assignTeamId != other.assignTeamId)
				return false;
			if(assigneeIds != other.assigneeIds)
				return false;
			if(supervisorIds != other.supervisorIds)
				return false;
			if(pipelineStepIds != other.pipelineStepIds)
				return false;
			if(issueTime != other.issueTime)
				return false;
			if(dueTime != other.dueTime)
				return false;
			if(isUnassigned != other.isUnassigned)
				return false;
			if(taskIds != other.taskIds)
				return false;
			if(isPendingAppend != other.isPendingAppend)
				return false;
			if(tagNames != other.tagNames)
				return false;
			if (alias != other.alias)
				return false;
			if(tagNameRegExp != other.tagNameRegExp)
				return false;
			if (excludeDeletedItem != other.excludeDeletedItem)
                return false;
			if(extraFilter != other.extraFilter)
				return false;
            return true;
		}

		bool operator!=(const TaskFilterParam& other)
		{
			return !(*this == other);
		}

		Serialization(fetchData, projectId, objectIds, types, status, assignTeamId, assigneeIds, supervisorIds, issueTime,
					  dueTime, isUnassigned, taskIds, pipelineStepIds, isPendingAppend, tagNames, alias, tagNameRegExp, excludeDeletedItem, extraFilter)
	};

	//资产过滤参数
    struct AssetFilterParam {
        enum FetchEnum {
            Basic = 1,
            ObjectName = 2,
            Task = 3,
            Tag = 4
        };

        QSet<qint32> fetchData = {};//请求号,用于指定获取的数据丰富程度

        qint32 projectId = 0;//根据项目id查询
        QVariantList assetIds;//根据项目的资产id查询
        qint32 status = 0;//根据资产的状态查询
        qint32 workFlowId = 0;//资产的workFlowId
        QDateTime issueTime;//根据资产的发布时间查询
        QDateTime dueTime;//根据资产的截至时间查询
        QString assetName = "";
        QString alias = ""; //别名
        QString searchKey = "";//资产名关键词查找

		QSet<QString> tagNames;//标签条件搜索
		qint32 tagSearchAndOr = 0; //搜索条件的逻辑关系。1 == and, 2 == or

        TaskFilterParam taskFilterParam;//资产过滤条件下有任务的过滤条件

        Serialization(fetchData, projectId, assetIds, status, workFlowId, issueTime, dueTime, assetName,alias,
                      searchKey, tagNames, tagSearchAndOr, taskFilterParam)
    };

    //镜头过滤参数
    struct ShotFilterParam {
        enum FetchEnum {
            Basic = 1,
            ObjectName = 2,
            Task = 3,
            Tag = 4
        };

        QSet<qint32> fetchData = {};//请求号,用于指定获取的数据丰富程度
        qint32 projectId = 0;//根据项目id查询
        QVariantList shotIds;//根据项目的资产id查询
        qint32 seasonId = 0;//TV 项目根据season获取
        qint32 episodeId = 0;//TV 项目，根据episode获取
        QVariantList sceneIds;//根据场景号来查找
        QString shotName;   //根据镜头名查找

        qint32 status = 0;//根据资产的状态查询
        qint32 workFlowId = 0;//资产的workFlowId
        QDateTime issueTime;//根据资产的发布时间查询
        QDateTime dueTime;//根据资产的截至时间查询
        QString searchKey = "";//镜头名关键词查找

        QSet<QString> tagNames;//标签条件搜索
		qint32 tagSearchAndOr = 0; //搜索条件的逻辑关系。1 == and, 2 == or

        TaskFilterParam taskFilterParam;//镜头过滤条件下有任务的过滤条件

        Serialization(fetchData, projectId, shotIds, seasonId, episodeId, sceneIds, shotName, status,
                      workFlowId, issueTime, dueTime, searchKey, tagNames, tagSearchAndOr, taskFilterParam)
    };

    //sequence过滤参数
    struct SequenceFilterParam {
        enum FetchEnum {
            Basic = 1,
            ObjectName = 2,
            Task = 3,
            Tag = 4
        };

        QSet<qint32> fetchData = {};//请求号,用于指定获取的数据丰富程度
        qint32 projectId = 0;//根据项目id查询
        QSet<qint32> sequenceIds;//根据项目的sequenceId查询
        qint32 seasonId = 0;//TV 项目根据season获取
        qint32 episodeId = 0;//TV 项目，根据episode获取
        QSet<qint32> sceneIds;//根据场景ID来查找

        qint32 workFlowId = 0;//workFlowId
        QDateTime issueTime;//根据sequence的发布时间查询
        QDateTime dueTime;//根据sequence的截至时间查询
        QString searchKey = "";//场景号关键词查找

        QSet<QString> tagNames;//标签条件搜索
		qint32 tagSearchAndOr = 0; //搜索条件的逻辑关系。1 == and, 2 == or

        TaskFilterParam taskFilterParam;//镜头过滤条件下有任务的过滤条件

        Serialization(fetchData, projectId,sequenceIds,seasonId, episodeId, sceneIds,
                workFlowId, issueTime, dueTime, searchKey, tagNames, tagSearchAndOr, taskFilterParam)
    };

	/*!
	 * \struct TaskFileRelationEditParam
	 * \ingroup project
	 * \brief 任务文件关系编辑接口
	 */
	struct TaskFileRelationEditParam
	{

		qint32 operationType = 0;//0：删除，1：添加， 2：编辑
		qint32 taskId = 0;//任务id

		//REFERENCE_FILE = 0, //引用文件
		//REQUIRED_FILE = 2, //所需文件
		//OUTPUT_FILE = 3 //输出文件
		qint32 fileType = 0;

		qint32 fileId = 0;//文件id

		qint32 variantId = 0;//variantId

		qint32 isIntermediate = 0;//是否是中间文件  0 否 1是

		qint32 isReview = 0;//是否为需要审批文件 0 否 1是

		QPair<QString, QString> path;//路径,一般是output file需要,first: 路径"/aa/bb/cc",second: aa.txt

		Serialization(operationType, taskId, fileType, fileId, path, variantId, isIntermediate, isReview)
	};

	/*!
     * \struct VariantVO
     * \ingroup
     * \brief
     */
	struct VariantEditParam {
		qint32 variantId = 0;//variantId
		QString variantName = "";//变种名

		QSet<qint32> fileIds;//文件的id

		Serialization(variantId, variantName, fileIds)
	};
	struct FileTaskVO;
	struct VariantVO {
		qint32 variantId = 0;
		QString variantName = "";
		qint32 assetId = 0;//资产id
		QList<FileTaskVO> taskFiles;

		Serialization(variantId, variantName, assetId, taskFiles)
	};

	struct VariantRecordVO {
		qint32 variantId = 0;
		QString variantName = "";//变种名
		QPair<qint32, QString> createdBy;//variant的创建者
		QList<INTask> tasks;//任务记录

		Serialization(variantId, variantName, createdBy, tasks)
	};

	/*!
	 * \enum FileTypeEnum
	 * \ingroup project
	 * \brief 文件类型的枚举
	 */
	enum class FileTypeEnum {
		ALL = -1,//所有的类型
		REFERENCE_FILE = 0, //引用文件
		REQUIRED_FILE = 2, //所需文件
		OUTPUT_FILE = 3 //输出文件
	};

	/*!
	 * \struct SequenceFileSetVO
	 * \ingroup file
	 * \brief 序列文件集视图对象
	 */
	struct SequenceFileSetVO {

		qint32 sequenceFileId = 0;//序列文件的id
		QString sequenceFileSetName = ""; //序列文件的名称
		QString directory = "";//序列文件的路径
		qint32 version = 0;//序列文件的版本
		QPair<qint32, QString> createBy;//创建者

		QList<FileVO> files;//所有的序列文件

		Serialization(sequenceFileId, sequenceFileSetName, directory, files, version, createBy)
	};


	/*!
	 * \struct Task
	 * \ingroup project
	 * \brief 任务实体类
	 */
	struct Task {

		qint32 taskId = 0;//任务id
		qint32 projectId = 0;//项目id
		QString taskName = "";
		qint32 type = 0;//0资产,1镜头
		qint32 assetOrShotId = 0;//资产或者镜头id
		QDateTime actualStartTime;//真正开始时间
		QDateTime actualEndTime;//真正结束时间
		bool isDeleted = false;//是否被删除
		qint32 status = 0;//任务状态
		QDateTime issueTime;//发布时间
		QDateTime dueTime;//任务结束时间
		QString storageType = "";//任务文件的存储类型
		qint32 coordinatorId = 0;
		qint32 supervisorId = 0;

		Serialization(taskId, projectId, taskName, type, assetOrShotId, actualStartTime, actualEndTime, isDeleted, status,
					  issueTime, dueTime, storageType, coordinatorId, supervisorId)
	};

	/*!
	 * \struct AffectedTaskInfo
	 * \ingroup project
	 * \brief 受影响的任务的信息
	 */
	struct AffectedTaskInfo {
		qint32 taskId = 0;//任务id
		QString taskName = "";//任务名

		qint32 projectId = 0;
		qint32 objectId = 0;
		qint32 assigneeId = 0;
		qint32 type = 0;

		QDateTime issueTime;//任务的计划开始时间
		QDateTime dueTime;//任务的计划结束时间

		QDateTime affectedIssueTime;//受影响之后的发布时间
		QDateTime affectedDueTime;//受影响之后的截至时间

		Serialization(taskId, taskName, projectId, objectId, assigneeId, type, issueTime, dueTime,
					  affectedIssueTime, affectedDueTime)
	};


	/*!
	 * \struct AffectedTaskVO
	 * \ingroup project
	 * \brief 受影响的task的前端视图对象
	 */
	struct AffectedObjectInfoVO {

		QMap<qint32, QString> affectedAssignees;//受影响的人员id信息
		QMap<qint32, QString> affectedAssets;//受影响的资产
		QMap<qint32, QString> affectedShots;//受影响的镜头
		QMap<qint32, QString> affectedProjects;//受影响的项目

		QMap<qint32, AffectedTaskInfo> affectedTaskInfos;//受影响的taskd的列表，key为taskId

		Serialization(affectedAssignees, affectedAssets, affectedShots,
					  affectedProjects, affectedTaskInfos)

	};


	/*!
	 * \struct TaskFileVO
	 * \ingroup project
	 * \brief 项目文件的视图对象
	 */
	struct TaskFileVO {

		qint32 taskId = 0;

		QList<FileVO> files; //任务包含的所有产出单文件

		QList<SequenceFileSetVO> sequenceFiles; //任务包含的所有序列文件

		QMap<qint32, Task> fileTaskMap;//文件文件对应的输出任务列表 key:文件id,value:任务信息

		QMap<qint32, QMap<qint32, QString>> fileVariantMap;//文件关联的variant信息。key fileId,value variantIds

		Serialization(taskId, files, fileVariantMap, sequenceFiles, fileTaskMap)
	};


	/*!
	 * \struct FileTaskVO
	 * \ingroup
	 * \brief
	 */
	struct FileTaskVO {
		INTask task;
		QList<FileVO> files;
		Serialization(task, files)
	};

	/*!
	 * \struct 存储信息的查询查询
	 * \ingroup
	 * \brief
	 */
	struct StorageInfoSearchParam {
		qint32 projectId = 0;
		qint32 taskId = 0;
		Serialization(projectId, taskId)
	};

	struct PanelReviewVO {
		qint32 panelReviewId = 0;
		qint32 projectId = 0;
		QString theme = "";
		QString place = "";
		QDateTime fromTime;
		QDateTime toTime;
		QList<IdName> participents;
		QPair<qint32, QString> holder;
		qint32 taskId = 0;
		qint32 status = 0;//会议状态[0：未进行/进行中，1：会议结束]
		//新添加
		QString projectName = "";//项目名
		QString taskName = "";
		qint32 type = 0;
		qint32 containerId = 0;//资产或者镜头的id
		QString containerName = "";//资产或者镜头的名称
		qint32 wrapperId = 0;//若是镜头,则表示场次的id
		QString wrapperName = "";//若是镜头,则表示镜头的名称

		Serialization(panelReviewId, projectId, theme, place, fromTime, toTime, participents, holder, taskId, status,
					  taskName, type, projectName, containerId, containerName, wrapperId, wrapperName)
	};

	struct PersonRoleDTO {
		qint32 entryId = 0;
		int roleId = 0;//角色id
		QString roleName;
		bool isDefault = false;//是否默认
		int grade = 0;//角色等级
		QString roleCode;//角色代码

		int personId = 0;//人员id
		QString userName;//用户名
		QString legalName;//真实姓名

		int teamId = 0;//团队id
		QString teamName;//团队名称

		int departmentId = 0;//部门id
		QString departmentName;//部门名称

		Serialization(entryId, roleId, roleName, isDefault, grade, roleCode,
					  personId, userName, legalName, teamId, teamName, departmentId, departmentName)
	};

	struct RecycleInfo
	{
		int         id = 0;    //主键
		int         projectId = 0;    //项目ID
		QString     projectName = "";   //项目名称
		int         type = 0;    //删除的业务所属类型（task/asset/shot） 0为task，1为asset，2为shot, 3为sequence，4为项目
		int         objectId = 0;    //type为0时,taskid;type为1时,assetid;type为2时,shotid;
		QString     objectName = "";   //名称
		int         deleteById = 0;    //删除者ID
		QString     deleteByName = "";   //删除者名字
		QDateTime   deleteTime;                 //删除时间

		Serialization(id, projectId, projectName, type, objectId, objectName, deleteById, deleteByName, deleteTime)
	};

	struct ProjectTaskCountItem {
		qint32 id = 0;
		QString name = "";
		qint32 allCount = 0;
		qint32 approvedCount = 0;

		Serialization(id, name, allCount, approvedCount);
	};

	struct ProjectTaskInfo {

		//项目信息
		qint32 projectId = 0;
		QString projectName = "";
		QDateTime projectIssueTime;
		QDateTime projectDueTime;

		int newlyTaskCount = 0;
		int leftDay = 0;//还剩下的时间
		int totalTaskCount = 0;//所有的任务数量

		//任务信息
		QMap<qint32, qint32> taskStatusInfoMap;


		//团队信息
		QMap<qint32, ProjectTaskCountItem> teamInfoMap;

		//人员任务信息 QPair<qint32, qint32> <总量，通过的量>
		QMap<qint32, ProjectTaskCountItem> personTaskInfoMap;


		Serialization(projectId, projectName, projectIssueTime, projectDueTime, newlyTaskCount, leftDay, totalTaskCount,
					  taskStatusInfoMap, teamInfoMap, personTaskInfoMap);
	};

	struct  PersonInActionParam {
		QSet<qint32> actions = {};  //权限点集合 如：1263， 1264
		qint32 AndOr = 0;           //权限点之间的逻辑关系，and == 1,or == 2
		qint32 projectId = 0;       //项目id

		Serialization(actions, AndOr, projectId)
	};

	struct WorkFlowDetlFile {
		qint32      workflowFileId = 0;    //主键ID
		qint32      workflowStepId = 0;    //work_flow_detl folderId
		qint32      parentWorkflowStepId = 0;    //父节点work_flow_detl folderId
		qint32      pathId = 0;
		QString     path = "";   //文件/文件夹路径
		qint32      type = 0;    //1表示文件，2表示文件夹
		QString     fileName = "";   //文件名
		qint32      fileId = 0;    //文件模板id
		qint32      fileType = 0;    //0=referenceFileId,1= repositoryFileId,2=requiredFileId,3=outFileId
		QDateTime   createTime;                     //创建时间

		Serialization(workflowFileId, workflowStepId, parentWorkflowStepId, pathId, path,
					  type, fileName, fileId, fileType, createTime)
	};

	class INQHolidayDetail
	{
	public:
		qint32      detailId            = 0;        //节假日ID
		qint32      templateId          = 0;        //公众假期模板id
		QString     holidayName;                    //节假日名称
		qint32      annual              = 0;        //年度
		QDate       startDate;                      //开始日期,格式：YYYY-MM-DD
		QDate       endDate;                        //结束日期,格式：YYYY-MM-DD
		QDateTime   createTime;
		Serialization(detailId, templateId, holidayName, annual,startDate,endDate,createTime)

		bool operator==(const INQHolidayDetail& other)
		{
			return detailId == other.detailId &&
			        templateId == other.templateId &&
			        holidayName == other.holidayName &&
			        annual == other.annual &&
			        startDate == other.startDate &&
			        endDate == other.endDate &&
			        createTime == other.createTime;
		}
	};

	//对应于服务端 INQHolidays
	class INQHolidayTemplate
	{
	public:
		qint32      templateId          = 0;            //公众假期模板id
		QString     templateName;                       //公众假期模板名
		QString     color;                              //颜色
		QList<INQHolidayDetail>   detailList;             //节假日列表

		Serialization(templateId, templateName, color, detailList)
	};

    struct TaskStatusItem
    {
        qint32                  taskStatusId    = 0;
        qint32                  status          = 0;    //状态值
        QString                 remark          = "";   //标记
        QDateTime               createTime;             //更新时间
        QPair<qint32, QString>  operatedBy;             //操作者

        Serialization(taskStatusId, status, remark, createTime, operatedBy)
    };

    struct TaskStage
    {
        qint32                  taskStatusId = 0;
        QPair<qint32, QString> operatedBy;
        QPair<qint32, QString>  from;
        QPair<qint32, QString>  to;
        qint32                  operation = 0;// 1 提交审批 2 reopen  3 next 4 prev
        QDateTime               createTime;
        Serialization(taskStatusId, operatedBy, from, to, operation,createTime)
    };

    struct TaskDurationProgress
    {
        qint32                  taskId              = 0;
        qint32                  reOpenCount         = 0;
        QDateTime               actualStartTime;            //实际开始时间
        QDateTime               actualEndTime;              //实际结束时间
        QDateTime               issueDate;                  //预计开始时间
        QDateTime               dueDate;                    //预计结束时间
        qint32                  daysConsumed        = 0;    //审批消耗总时长 （单位：天）
        QList<TaskStatusItem>   taskStatusList;
        QList<TaskStage>        taskStageList;

        Serialization(taskId,reOpenCount,actualStartTime,actualEndTime,issueDate,
                      dueDate,daysConsumed,taskStatusList,taskStageList)
    };

    struct DataAuthVO {
        qint32 dataAuthId = 0;
        qint32 personId = 0;
        qint32 roleId = 0;
        qint32 actionId = 0;
        qint32 resourceId = 0;

        Serialization(dataAuthId, personId, roleId, actionId, resourceId);
    };

    // --------------------------------------

    struct TaskLinkFile
    {
		qint32      taskFileId = 0;  //主键
		qint32      taskId = 0;  //任务id
		qint32      parentTaskId = 0;  //父节点
		qint32      fileId = 0;  //文件id
		QString     path = ""; //文件/文件夹路径
		qint32      type = 0;  //1表示文件，2表示文件夹
		QString     fileName = ""; //文件名
		qint32      fileType = 0;  //0=referenceFileId,1= repositoryFileId,2=requiredFileId,3=outFileId
		qint32      buildType = 0;  //生成类型[0：系统根据上下游关系生成的，1：用户后面手动添加]
		qint32      isIntermediate = 0;  //是否为中间文件 0否 1是
		qint32      isReview = 0;  //是否为审批文件 0否 1是

		Serialization(taskFileId, taskId, parentTaskId, fileId, path,
			type, fileName, fileType, buildType, isIntermediate, isReview)
    };

    struct TaskWorkflowDetail
    {
        qint32 taskId = 0; // 任务ID
        QString taskName; // 任务名
		QString alias; //别名
        QString color; // 节点颜色
        QPair<QPoint, QPoint> position; // 节点位置
        QList<qint32> parentTaskIdList; // 父亲节点列表
        qint32 status = 0; // 任务状态
        qint32 taskType = 0; // 节点类型 0-普通任务节点 1-容器
        qint32 containerId = 0;
//        QList<TaskLinkFile> taskFileList;
//        qint32 operation = 0; // 用户操作 -1 - 删除 0-默认 1-新增
        qint32 pipelineStepId = 0;

        Serialization(taskId, taskName, alias, color, position, parentTaskIdList, status,
                      taskType, containerId, /*taskFileList, operation,*/ pipelineStepId);
    };

    struct TaskWorkflow
    {
        qint32 projectId = 0; // 项目ID
        QString projectName; // 项目名称
        qint32 objectId = 0; // 资产或镜头ID
        qint32 objectType = 0; // 1 - 资产 2-镜头
        QString objectName; // 资产名或镜头号
        QList<TaskWorkflowDetail> taskList; // 任务节点列表

        Serialization(projectId, projectName, objectId, objectType, objectName, taskList);
    };

	struct InstanceTaskRequest
	{
		qint32      projectId       = 0;    //项目ID
		qint32      pipelineStepId  = 0;    //pstId
		QString     taskName        = "";   //任务名字
		qint32      objectId        = 0;    //资产/镜头Id
		QString     color           = "#FFFFFF"; //节点颜色
		QPair<QPoint, QPoint> position;   //节点位置

		Serialization(projectId,pipelineStepId,taskName,objectId,color,position);
	};

    struct ProjectStatisticsData {
        qint32 totalScenes = 0;
        qint32 totalAsset = 0;
        qint32 totalShot = 0;
        qint32 totalSequence = 0;
        qint32 totalTask = 0;
        qint32 taskToDo = 0;
        qint32 taskWIP = 0;
        qint32 taskPendingApproval = 0;
        qint32 taskReopened = 0;
        qint32 taskPendingValidation = 0;
        qint32 taskApproved = 0;
        qint32 taskPaused = 0;
        qint32 taskDeprecated = 0;
        qint32 taskOnHold = 0;
        qint32 taskUnassigned = 0;

        Serialization(totalTask, totalAsset, totalShot, totalSequence, totalScenes, taskToDo, taskWIP, taskPendingApproval, taskReopened,
                      taskPendingValidation, taskApproved, taskPaused, taskDeprecated, taskOnHold, taskUnassigned)
    };

    struct INSceneVO {
        IdName scene;   //scene id 和 name
        IdName project; //所属的项目id 和 name
        IdName season;  //所属的季id 和 name, 用于tv类项目
        IdName episode; //所属的集id 和 name, 用于tv类项目

        Serialization(scene, project, season, episode)
    };

    enum class ProjectType {
        None = 0,
        FILM = 1,
        TV = 2,
        MAX = 3
    };

    struct SearchTagParam {

        qint32 projectId = 0;
        QSet<qint32> sceneIds;
        QSet<qint32> objectIds;
        Serialization(projectId, sceneIds, objectIds)
        //标签对象的枚举(即objectId),要求数据库表按此规范
//        enum class TagObjectEnum {
//            None = 0,
//            PROJECT = 1,
//            WORK_FLOW = 2,
//            ASSET = 3,
//            SHOT = 4,
//            TASK = 5,
//            FOLDER = 6,
//            FILE = 7
//        };
    };


	/*!
	 * \struct
	 * \ingroup
	 * \brief 路径
	 */
	struct PipelineStepPathMappingVO {

		enum {
			SELECTED = 0,//被选中的
			UNSELECTED = 1,//未被选中的
			DELETED = 2//被删除的
		};

		qint32 pathId = 0;
		QString path = "";
		QString fileName = "";
		qint32 pipelineStepId = 0;

		qint32 flag = SELECTED;//标志，被选中，未被选择，被删除

		Serialization(pathId, path, fileName, pipelineStepId, flag)
	};

	/*!
	 * \struct
	 * \ingroup
	 * \brief 匹配
	 */
	struct PipelineStepMappingVO {

		qint32 mappingId = 0;//映射的id
		QString mappingName = "";//映射名
		bool isValid = true;//是否有效
		QString message = "";//错误信息

		QPair<qint32, QString> pipelineStep;//左边的pipelinestep

		QPair<qint32, QString> createBy;
		QDateTime createTime;

		QMap<qint32, QString> matchedPipelineStep;//匹配的对应的pipelinestep key: pipelinestepId, value: pipelineName

		QList<PipelineStepPathMappingVO> matchedPipelineStepPaths;//匹配到的pipelinestep路径

		Serialization(mappingId, mappingName, isValid, message, pipelineStep, createBy, createTime, matchedPipelineStep,
					  matchedPipelineStepPaths)
	};

	/*!
	 * \struct
	 * \ingroup
	 * \brief 编辑参数
	 */
	struct PipelineStepMappingEditParam {

		qint32 pipelineStepId = 0;//当前pipelinestep的id
		QSet<qint32> matchedPipelineStepPathIds;//pipelineStep的路径的id
		QSet<qint32> matchedPipelineStepIds;//匹配的pipelinestep的id

		Serialization(pipelineStepId, matchedPipelineStepPathIds, matchedPipelineStepIds)
	};

    /**
 * 文件的匹配状态
 */
    struct TaskPipelineStepFileMatchedVO {
        QString filePath = "";//文件路径
        QString fileName = "";//文件名字

        qint32 matchedState = 0;//是否匹配

        Serialization(filePath, fileName, matchedState)
    };

    struct TaskPipelineStepVO {
        qint32 taskId = 0;
        QString taskName = "";
        qint32 pipelineStepId = 0;
        QString pipelineName = "";

        qint32 matchedState = 0;//总体来说的匹配情况
        QList<TaskPipelineStepFileMatchedVO> taskPipelineStepFileMatchedVOs;//需要匹配的文件信息

        Serialization(taskId, taskName, pipelineStepId, pipelineName, matchedState, taskPipelineStepFileMatchedVOs)
    };

	struct TaskPipelineStepMappingStateVO {
		enum {
			NORMAL = 1,
			MATCHED = 2,
			MIS_MATCHED = 4,
			DUPLICATE_MATCHED = 8
		};

		TaskPipelineStepVO shotTaskPipelineStep;

		//key 匹配情况枚举，value 涉及到的任务
        QList<TaskPipelineStepVO> assetTaskPipelineStepState;

		qint32 matchedState = 0;//总体来说的匹配情况

		Serialization(shotTaskPipelineStep, assetTaskPipelineStepState, matchedState)
	};

	struct AssetVariantSimpleInfoVO {
		qint32 variantId = 0;
		QString variantName = "";//variant名字
		qint32 assetId = 0;
		QString assetName = "";//资产名

		Serialization(variantId, variantName, assetId, assetName)
	};

    struct ShotAssetMappingView {

        enum {
            NORMAL = 0,
            DISABLE
        };

        qint32 shotId = 0;//镜头id
        QString shotName = "";//镜头名字

        QList<QPair<AssetVariantSimpleInfoVO, qint32>> variantMatchedPairs;//variant简单匹配信息

        qint32 status = NORMAL;//状态: 0 NORMAL, 1: DISABLE

        Serialization(shotId, shotName, variantMatchedPairs, status)
    };

	/**
	 * 批量文件下载请求
	 */
	struct FileDownloadItem {
		qint32 fileId = 0;      //文件id
		qint32 version = 0;     //文件版本号，特殊值含义： -1表示获取该文件所有版本信息，0表示获取当前最新版本信息

		Serialization(fileId, version)
	};

	struct FileDownloadRequestParam {
		qint32 taskId = 0;    //任务ID，下载任务关联文件时必填。非任务关联文件可填
		QList<FileDownloadItem> fileList;

		Serialization(taskId, fileList)
	};

	/**
	 * 批量文件下载应答
	 */
	struct FileStorageInfo {
		QString ip = ""; //存储服务器IP地址
		qint32 port = 0; //存储服务器的端口
		QString path = "";//文件服务器的文件路径
		QString name = "";//文件在存储服务器的文件名称,一般跟应用服务器的文件名不相同
		FileVersionVO fileVersionVo; //文件版本信息

		Serialization(ip, port, path, name, fileVersionVo)
	};
	struct FileDownloadResponseParam {
		qint32  fileId = 0;                //文件id
		QString token = "";               //文件传输的校验码
		QList<FileStorageInfo>  fileList;   //文件不同版本对应存储服务器信息
		FileVO  clientInfo;                 //文件基本信息

		Serialization(fileId, token, fileList, clientInfo)
	};

	/**
	 * 批量文件上传请求
	 */
	struct FileUploadItem {
		qint32  fileId = 0;    //文件ID。填任务关联文件ID，如果为新文件可以不填。
		qint32  fileType = 0;    //文件类型。任务关联文件必填，其他可以不填。 0 referenceFile;1 repositoryFile(废弃);2 requiredFile; 3 outputFile
		QString path = "";   //文件相对路径。由客户端处理
		QString fileName = "";   //文件名字
		QString checkCode = "";   //文件md5

		Serialization(fileId, fileType, path, fileName, checkCode)
	};

	struct FileUploadRequestParams {
		qint32 projectId = 0; //项目ID  任务关联文件必填，其他类型文件可填
		qint32 taskId = 0; //任务id  任务关联文件必填，其他类型文件可填
		qint32 assetId = 0; //资产ID，上传资产参考文件必填，其他可填
		qint32 shotId = 0; //镜头ID，上传镜头参考文件必填，其他可填
		qint32 uploadType = 0; //上传类型。 0 任务关联文件; 1 头像文件; 2 placeholder文件 ; 3 评论文件; 4 library文件(for python api); 5 中间文件; 6 资产/镜头参考文件
		QList<FileUploadItem> files;//文件信息

		Serialization(projectId, taskId, assetId, shotId, uploadType, files)
	};

	/**
	 * 批量文件上传应答
	 */
	struct TransferFileInfo {
		FileVO file;//对应的文件信息
		MessageInfo message;//对应的成功失败信息, messageInfo: 1：成功，2：已存在，不用上传 3：没有权限，4：其他错误
		QString token = "";//文件传输的校验码

		Serialization(file, message, token)
	};
	struct FileUploadResponseParams {
		QString ip = ""; //存储服务器IP地址
		qint32 port = 0; //存储服务器的端口
		QList<TransferFileInfo> fileInfos;//上传的任务下的文件的客户端信息

		Serialization(ip, port, fileInfos)
	};

	struct INReviewProject {
		IdName project;					//项目名和项目id
		qint32 totalAssetTask = 0;		//项目下资产的reopen状态和pending approval状态的任务总数
		qint32 totalShotTask = 0;		//项目下镜头的reopen状态和pending approval状态的任务总数
		qint32 totalSequenceTask = 0;	//项目下seq的reopen状态和pending approval状态的任务总数
		Serialization(project, totalAssetTask, totalShotTask, totalSequenceTask);
	};

	struct INTaskPreview {
		IdName file;
		QString checkCode;
		qint32 version = 0;
		qint64 size = 0;

		Serialization(file, checkCode, version, size);
	};

    struct INReviewTask {
		IdName project;              //任务所属的项目
        QString tag;               //例如 “ep001_a001”
		QString objectName;          //镜头号或者资产名，用于tab标题
        QString taskPath;            //对于资产：projectName,对于镜头：projectName<_season><_episode>_scene,对于sequence：projectName<_season><_episode>
        QString reviewTaskName;      //review模块的任务名。reviewTaskName = task.name - taskPath
        QString batchTaskCardName;        // 批量界面任务卡的名字
		IdName task;                 //任务id和名
		IdName pipelineStep;         //任务所属的pipeline step的id和名
		IdName assignee;             //完成此任务的人
		qint32 status = 0;           //任务状态，这里只有reopen和pending approval状态
		qint32 type = 0;             //任务类别 0：asset 1：shot 2：seq
		QDateTime statusUpdatingTime;   //任务状态改变的时间
		QDateTime dueTime;          //任务结束时间
		qint32 leftTime = 0;        //到任务结束还剩余的时间,单位天，负值表示超时
		QList<TaskApproval> approvalList;   //任务的审批列表
		TaskApproval currentApprover;       //任务的当前审批节点
		INTaskPreview taskPreview;  //任务的预览图，用审批的第一个文件。

        Serialization(project, tag, objectName, taskPath, reviewTaskName,batchTaskCardName,task, pipelineStep, assignee, status,
			type, statusUpdatingTime, dueTime, leftTime, approvalList, currentApprover, taskPreview);
	};

    struct GetReviewTaskParam {
        qint32 projectId = 0;   //指定审批任务所属的项目
        qint32 objectType = 0;  //指定审批任务所属的类型，0 == asset，1 == shot, 2 == seq
        qint32 orderBy = 0;     //排序方式：默认time，1 == pipeline step name
        qint32 taskStatus = 0;  //按照任务的状态筛选

        Serialization(projectId, objectType, orderBy, taskStatus);
    };

	struct INReviewTaskList {
		QString key;        //每列左上角关键字
		QList<INReviewTask> tasks;  //任务列表

		Serialization(key, tasks);
	};

	struct INFileReviewInfo {

		qint32 historyVersionReviewed = 0;  //0：历史版本没有review信息，其他：有
		QMap<qint32, QList<QString>> reviewList;  //播放器上的涂涂改改.key:文件版本，value:内容

		Serialization(historyVersionReviewed, reviewList);
	};

	struct INTaskReviewFile {
		INReviewTask reviewTask;    //要审批的任务
		QMap<qint32, FileVO> reviewFiles;      //任务下要审批的文件。key: fileId, value:交给前端的file vo结构体
		QMap<qint32, QString> notForReviewFiles;    //任务下不需要审批的文件key: fileId, value: 仅名字，包括后缀名，无路径

		//审批文件的审批信息。key:file id, file version, value:文件的审批信息
		QMap<qint32, INFileReviewInfo> fileReviewInfo;

		Serialization(reviewTask, reviewFiles, notForReviewFiles, fileReviewInfo);
	};

    struct INPostReviewParam {
		qint32 taskId = 0;      //评论的任务的Id，如果是播放器上涂鸦的内容，可不填此Id
		qint32 fileId = 0;      //评论的文件的Id,如果不是播放器上涂鸦的内容，可不填此Id。
		qint32 fileVersion = 0; //评论文件时，须指定版本。
		QString content;        //视频播放器上涂鸦的内容
		QString userText;       //用户输入的文本内容，包括视频播放器正下方post框生成的内容。
		QSet<qint32> audioIds;  //语音评论文件id
		QSet<qint32> videoIds;  //视频评论文件id
		QSet<qint32> pixmapIds; //图片评论文件id

		Serialization(taskId, fileId, fileVersion, content, userText, audioIds, videoIds, pixmapIds);
    };

	struct INReviewTaskComment {
		IdName person;          //评论人
		qint32 isApprover = 0;  //评论人是否为审批者标记
		qint32 avatar = 0;      //评论人头像文件Id

		//评论的类型，用于区别任务操作日志使用不同图标。默认类别0下为通常的任务评论。
		//1:任务创建，2：任务状态变化，3：任务分派，4：send to next review 5: send to previous review 6: time change
		qint32 type = 0;
		QString text;           //评论内容文本。
		qint32 commentId = 0;   //评论的Id.用于获取对应的涂鸦时起效。其他情况为0
		QDateTime submitDate;   //评论产生的时间
		QSet<qint32> audioIds;  //语音评论id
		QSet<qint32> videoIds;  //视频评论id
		QSet<qint32> pixmapIds; //图片评论id

		Serialization(person, isApprover, avatar, type, text, commentId, submitDate, audioIds, videoIds, pixmapIds);
	};

    struct PipelineStepExport {

        IdName sourceProject;
        QMap<qint32, INPipelineStep> pipelineSteps;
        QMap<qint32, INQWorkPath> pipelineStepWorkPath;
        QMap<qint32, INQWorkFlowVO> workFlows;
        QMap<qint32, WorkFlowDetlFile> workFlowFiles;
        QMap<qint32, PipelineStepMappingVO> pipelineStepMappingVOs;

        Serialization(sourceProject, pipelineSteps, pipelineStepWorkPath, workFlows, workFlowFiles,
                      pipelineStepMappingVOs)
    };

    struct TaskOutputFile {

        qint32 fileId = 0;
        qint32 isIntermediate = 0;
        qint32 isReview = 0;
        QString path;   //文件所在的目录，不包括文件名。以"/"开头，但不以"/"结尾。例如：/ab/cd/ef
        QString name;   //文件名,包括后缀。例如：abc.txt

        Serialization(fileId, isIntermediate, isReview, path, name);
    };
};
