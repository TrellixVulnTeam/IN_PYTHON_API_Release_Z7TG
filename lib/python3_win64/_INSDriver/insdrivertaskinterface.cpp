﻿#include "insdrivertaskinterface.h"
#include "inscommonhandle.h"

namespace INS_INTERFACE {
    INSDRIVER_EXPORT MessageInfo
    GetTaskSequenceFile(QMap<QString, QList<SequenceFileSetVO>> &sequenceFileInfos, qint32 taskId, qint32 type) {
        INSCommonRequest<QMap<QString, QList<SequenceFileSetVO>>> commonRequest(5208, taskId, type);
        commonRequest.WaitForFinished();
        sequenceFileInfos = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo SaveTaskSequenceFile(const SequenceFileParam &param) {
        INSCommonRequest<qint32> commonRequest(5207, param);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo
    GetAffectedTaskInfo(AffectedObjectInfoVO &affectedInfos, qint32 taskId, const QDateTime &dueTime) {
        INSCommonRequest<AffectedObjectInfoVO> commonRequest(5204, taskId, dueTime);
        commonRequest.WaitForFinished(-1);
        affectedInfos = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo
    GetTaskFileUsageRecord(QList<FileTaskVO> &fileTaskVOs, qint32 taskId, qint32 projectId, qint32 taskType) {
        INSCommonRequest<QList<FileTaskVO>> commonRequest(5203, taskId, projectId, taskType);
        commonRequest.WaitForFinished();
        fileTaskVOs = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT qint32 GetTasksByConditions(QList<INTask> &tasks, TaskFilterParam &params) {
        INSCommonRequest<QList<INTask>> commonRequest(5202, params);
        commonRequest.WaitForFinished();
        tasks = commonRequest.getRecvData();
        return commonRequest.getRetValue().code;
    }

    INSDRIVER_EXPORT MessageInfo
    GetTaskRelatedFiles(QMap<qint32, TaskFileVO> &taskFileVOs, qint32 taskId, FileTypeEnum fileType) {
        INSCommonRequest<QMap<qint32, TaskFileVO>> commonRequest(5200, taskId, (qint32) fileType);
        commonRequest.WaitForFinished();
        taskFileVOs = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo
    EditTaskFileRelatedFiles(const QList<TaskFileRelationEditParam> &taskFileRelationEditParams) {
        INSCommonRequest<qint32> commonRequest(5201, taskFileRelationEditParams);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo
    GetNewTaskId(const InstanceTaskRequest& option) {
        INSCommonRequest<qint32> commonRequest(5223, option);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    //获取下一个可显示的状态
    INSDRIVER_EXPORT MessageInfo GetTaskNextAvailableStatus(QList<qint32> &taskStatus, int taskId) {
        INSCommonRequest<QList<qint32>> commonRequest(587, taskId);
        commonRequest.WaitForFinished();
        taskStatus = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

	INSDRIVER_EXPORT MessageInfo GetTasksNextAvailableStatus(QList<qint32>& taskStatus, const QSet<qint32>& taskIds) 
	{
		INSCommonRequest<QList<qint32>> commonRequest(588, taskIds);
		commonRequest.WaitForFinished();
		taskStatus = commonRequest.getRecvData();
		return commonRequest.getRetValue();
	}

    //获取pannelView的人员 584
    INSDRIVER_EXPORT MessageInfo GetPannelViewPersons(QList<PersonRoleDTO> &personRoles, int projectId, int taskId) {
        INSCommonRequest<QList<PersonRoleDTO>> commonRequest(584, projectId, taskId);
        commonRequest.WaitForFinished();
        personRoles = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    //获取人员的会议列表 585
    INSDRIVER_EXPORT MessageInfo GetPannelViewList(QList<PanelReviewVO> &panelViews, int projectId) {
        INSCommonRequest<QList<PanelReviewVO>> commonRequest(585, projectId);
        commonRequest.WaitForFinished();
        panelViews = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    //创建或者修改pannel review的列表 586
    INSDRIVER_EXPORT MessageInfo CreatePannelReviewOrModify(PanelReviewVO &panelView) {
        INSCommonRequest<PanelReviewVO> commonRequest(586, panelView);
        commonRequest.WaitForFinished();
        panelView = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    //获取文件服务器placeholder列表
    INSDRIVER_EXPORT qint32 GetPlaceHolderList(QList<FileVO> &placeholderlist) {
        INSCommonRequest<QList<INS::FileVO>> phRequest(351, 0);
        phRequest.WaitForFinished();
        placeholderlist = phRequest.getRecvData();
        return phRequest.getRetValue().code;
    }

    //创建任务
    INSDRIVER_EXPORT MessageInfo
    CreateTask(INTask &newTask) {
        INSCommonRequest<INTask> commonRequest(5210, newTask);
        commonRequest.WaitForFinished();
        newTask = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    //编辑任务信息
    INSDRIVER_EXPORT MessageInfo EditTask(INTask &etask) {
        INSCommonRequest<INTask> commonRequest(523, etask);
        commonRequest.WaitForFinished();
        etask = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo
        EditTaskTime(const QSet<qint32> &taskIds, const QDateTime &issueDate, const QDateTime &dueDate) {
        INSCommonRequest<qint32> commonRequest(527, taskIds, issueDate, dueDate);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo EditTaskAssignee(const QSet<qint32> &taskIds, qint32 assignee, QMap<QString, QString> &failTasks) {
        INSCommonRequest<QMap<QString, QString>> commonRequest(528, taskIds, assignee);
        commonRequest.WaitForFinished();
        failTasks = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    ///编辑任务审批流程
    INSDRIVER_EXPORT MessageInfo EditTaskApproval(QList<TaskApproval> &approvalList) {
        INSCommonRequest<QList<TaskApproval>> commonRequest(5206, approvalList);
        commonRequest.WaitForFinished();
        approvalList = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    ///任务审批流程操作
    INSDRIVER_EXPORT MessageInfo approveTaskApprovalStatus(qint32 taskId, qint32 direction) {
        INSCommonRequest<qint32> commonRequest(533, taskId, direction);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    ///任务审批流程加急审批
    INSDRIVER_EXPORT MessageInfo setTaskApprovalToUrgent(qint32 taskId, qint32 approverID) {
        INSCommonRequest<qint32> commonRequest(534, taskId, approverID);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    MessageInfo setBatchTaskApprovalList(TaskApprovalWorkFlow& taskApprovalWorkFlow)
    {
        INSCommonRequest<TaskApprovalWorkFlow> commonRequest(5212, taskApprovalWorkFlow);
        commonRequest.WaitForFinished();
        taskApprovalWorkFlow = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    MessageInfo saveTaskOutPutFile(qint32 taskId, const QList<TaskOutputFile> &taskOutputFiles)
    {
        INSCommonRequest<qint32> commonRequest(5215, taskId, taskOutputFiles);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo saveTaskRelationFile(const TaskRelationFileRequest& request){
        INSCommonRequest<qint32> commonRequest(5215, request);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT MessageInfo GetTaskOutputFileComment(qint32 taskId, QList<FileComment>& fileComments) {
		INSCommonRequest<QList<FileComment>> commonRequest(5216, taskId);
		commonRequest.WaitForFinished();
        fileComments = commonRequest.getRecvData();
		return commonRequest.getRetValue();
    }
	INSDRIVER_EXPORT MessageInfo GetTaskDefaultOutputPath(qint32 taskId, QString& path) {
		INSCommonRequest<QString> commonRequest(5217, taskId);
		commonRequest.WaitForFinished();
        path = commonRequest.getRecvData();
		return commonRequest.getRetValue();
    }
	INSDRIVER_EXPORT MessageInfo getOutputFiles(const OutputFileRequest& request, OutputFileResponse& response) {
		INSCommonRequest<OutputFileResponse> commonRequest(5218, request, response);
		commonRequest.WaitForFinished();
        response = commonRequest.getRecvData();
		return commonRequest.getRetValue();
    }
	INSDRIVER_EXPORT MessageInfo getRequiredFiles(qint32 taskId, QList<OutputFileResponse>& responseList) {
		INSCommonRequest<QList<OutputFileResponse>> commonRequest(5219, taskId);
		commonRequest.WaitForFinished();
        responseList = commonRequest.getRecvData();
		return commonRequest.getRetValue();
    }



    ///任务审批流程的获取
    INSDRIVER_EXPORT MessageInfo getTaskApprovalList(qint32 taskId, QList<TaskApproval> &approvalList) {
        INSCommonRequest<QList<TaskApproval>> commonRequest(5205, taskId);
        commonRequest.WaitForFinished();
        approvalList = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }


    INSDRIVER_EXPORT QList<INTaskStatus> GetTaskStatusList() {
        return InTaskStatusOperate::GetTaskStatusList();
    }

    INSDRIVER_EXPORT QString GetTaskStateString(INTaskStatus taskState) {
        return InTaskStatusOperate::GetINTaskStausString(taskState);
    }

    //改变任务状态
    INSDRIVER_EXPORT MessageInfo UpdateTaskStatus(qint32 taskId, qint32 status, bool isRequiredFirstApproved) {
        INSCommonRequest<qint32> commonRequest(525, taskId, status, isRequiredFirstApproved);

        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT qint32 FindTaskStatus(TaskDurationProgress &progress, qint32 taskId) {
        INSCommonRequest<TaskDurationProgress> commonRequest(526, taskId);
        commonRequest.WaitForFinished();
        progress = commonRequest.getRecvData();
        return commonRequest.getRetValue().code;
    }

    //添加文件记录到taskFiles表
    INSDRIVER_EXPORT qint32 AddRecordToTaskFiles(int taskId, int fileId, int filetype, int isauto) {return 0;}

    INSDRIVER_EXPORT qint32 SubmitComment(INTaskComment &taskComment) {
        INSCommonRequestRetInt<INTaskComment> commonRequest(531, taskComment);
        commonRequest.WaitForFinished();
        taskComment = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT qint32 GetTaskComment(qint32 taskId, QList<INTaskComment> &taskComments, qint32 taskStateId) {
        INSCommonRequestRetInt<QList<INTaskComment>> commonRequest(532, taskId, taskStateId);
        commonRequest.WaitForFinished();
        taskComments = commonRequest.getRecvData();
        return commonRequest.getRetValue();
    }

    INSDRIVER_EXPORT qint32 GetTaskListPersonal(qint32 personId, QList<INTask> &tasklist) {
        TaskFilterParam params;
		params.assigneeIds = { personId };
        return GetTasksByConditions(tasklist, params);
    }

	//获取当前任务文件历史表
	INSDRIVER_EXPORT qint32 GetFileHistoryList(int taskId, QList<FileVO> &filehistorylist) {
		QMap<qint32, TaskFileVO> taskFileVOs;
		auto ret= INS_INTERFACE::GetTaskRelatedFiles(taskFileVOs, taskId,INS::FileTypeEnum::OUTPUT_FILE);
		for (auto& tf : taskFileVOs) {
			filehistorylist.append(tf.files);
			for (auto& sf:tf.sequenceFiles) {
				filehistorylist.append(sf.files);
			}
		}

		for(int i = 0; i < filehistorylist.size(); ++i)
        {
		    if (filehistorylist[i].fileId < 0 || filehistorylist[i].type == -1)
		        filehistorylist.removeAt(i--);
        }
		return ret.code;
	}

    INSDRIVER_EXPORT qint32 GetTaskFromID(qint32 taskId, INTask &task) {
        QList<INTask> tasks;
        TaskFilterParam params;
        params.taskIds = {taskId};
        auto ret = GetTasksByConditions(tasks, params);
        task = tasks.isEmpty() ? INTask() : tasks[0];
        return ret;
    }

    INSDRIVER_EXPORT qint32 GetAppendTask(qint32 pId, QList<INTask> &tasklist) {
        TaskFilterParam params;
        params.isPendingAppend = true;
        return GetTasksByConditions(tasklist, params);
    }

    INSDRIVER_EXPORT qint32 DeleteTask(qint32 taskId) {
        INSCommonRequestRetInt<qint32> commonRequest(524, taskId);
        commonRequest.WaitForFinished();
        return commonRequest.getRetValue();
    }
};