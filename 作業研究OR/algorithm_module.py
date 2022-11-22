# from ntpath import join
# from posixpath import split
# from tkinter.tix import Tree
# from turtle import Turtle, left


def heuristic_algorithm(file_path):
    '''
    1. Write your heuristic algorithm here.
    2. We would call this function in CA2_grading_program.py to evaluate your algorithm.
    3. Please do not change the function name and the file name.
    4. The parameter is the file path of a data file, whose format is specified in the document.
    5. You need to return your schedule in two lists "machine" and "completion_time".
        (a) machine[j][0] is the machine ID of the machine to process the first stage of job j + 1, and
            machine[j][1] is the machine to process the second stage of job j + 1.
        (b) completion_time[j][0] is the completion time of the first stage of job j + 1, and
            completion_time[j][1] is the completion time of the second stage of job j + 1.
        Note 1. If you have n jobs, both the two lists are n by 2 (n rows, 2 columns).
        Note 2. In the list "machine", you should record the IDs of machines
                (i.e., to let machine 1 process the first stage of job 1,
                you should have machine[0][0] == 1 rather than machine[0][0] == 0).
    6. You only need to submit this algorithm_module.py.
    '''

    import csv
    # ID ,processing time, valid machine, due time, stage
    # cmp is the value to compare

    class Jobs:
        def __init__(self, ID, pt1, pt2, pm1, pm2, due, phase):
            self.ID = ID
            self.phase = phase
            self.pt1 = pt1
            self.pt2 = pt2
            self.pm1 = pm1
            self.pm2 = pm2
            self.due = due
            self.cmp = due-pt1-pt2
    jobs = []
    # read data and store the information into your self-defined variables
    fp = open(file_path, 'r', newline='')
    header = fp.readline()
    reader = csv.reader(fp, delimiter=',')
    machine_num = 0
    for a_row in reader:
        # print(a_row)  # a_row is a list
        tmp = [item for item in a_row[3].split(',') if item != "N/A"]
        if(tmp):
            machine_num = int(max(tmp))
        tmp = [item for item in a_row[4].split(',') if item != "N/A"]
        if(tmp):
            machine_num = int(max(tmp))
        jobs.append(Jobs(int(a_row[0]), float(a_row[1]), float(a_row[2]),
                    a_row[3].split(','), a_row[4].split(','), float(a_row[5]), "Not allocated"))

    # start your algorithm here
    machine = []
    completion_time = []
    accu_machine = []
    stage_2 = []
    tardy = []
    for i in range(machine_num):
        accu_machine.append((i+1, 0))

    for i in range(len(jobs)):
        machine.append([0, 0])
        completion_time.append([0, 0])

    while(jobs):
        ptr = 0
        # sort joblists & machine accumulated time
        jobs = sorted(jobs, key=lambda job: (job.cmp, job.ID))
        accu_machine = sorted(
            accu_machine, key=lambda accu: (accu[1], accu[0]))
        job_now = jobs[0]
        # 如果出現了 cmp已經負數，那就沒可能做 直接丟tardy
        if(job_now.cmp < 0):
            tardy.append(jobs[0])
            jobs.pop(0)
            continue
        # 如果還沒分配過 分配stage 1 並把cmp 改成 due - pt2
        if(job_now.phase == "Not allocated"):
            while(str(accu_machine[ptr][0]) not in job_now.pm1 and ptr < len(accu_machine)):
                ptr += 1
            machine[job_now.ID - 1][0] = accu_machine[ptr][0]
            completion_time[job_now.ID -
                            1][0] = accu_machine[ptr][1] + job_now.pt1
            # accu the time
            tmpL = list(accu_machine[ptr])
            tmpL[1] += job_now.pt1
            accu_machine[ptr] = tuple(tmpL)
            job_now.phase = "Stage 1"
            job_now.cmp = job_now.due - job_now.pt2
            # 檢查有沒有stage 2
            if('N/A' not in job_now.pm2):
                stage_2.append(job_now)
                jobs[0] = job_now
            else:
                jobs.pop(0)
        # 如果分配到stage1過了，那就分配stage2上去並把job丟掉 (完結他)
        elif(job_now.phase == "Stage 1"):
            while(str(accu_machine[ptr][0]) not in job_now.pm2 and ptr < len(accu_machine)):
                ptr += 1
            machine[job_now.ID - 1][1] = accu_machine[ptr][0]
            completion_time[job_now.ID -
                            1][1] = accu_machine[ptr][1] + job_now.pt2
            tmpL = list(accu_machine[ptr])
            tmpL[1] += job_now.pt2
            accu_machine[ptr] = tuple(tmpL)
            jobs.pop(0)
            # stage 2不再需要出現這個job
            for i, j in enumerate(stage_2):
                if j.ID == job_now.ID:
                    del stage_2[i]
                    break
        ptr = 0
        # To know
        emergency = True
        solved = False
        # sort stage2
        while(emergency and stage_2):
            emergency = False
            stage_2 = sorted(
                stage_2, key=lambda job: (job.cmp, job.pt2*-1))
            job2_now = stage_2[0]
            # id, accu_time
            # if emergency happens, check that machine.
            for k, (i, j) in enumerate(accu_machine):
                a = max(2, len(stage_2)/len(accu_machine))
                # make sure it can be done by the machine
                if(str(i) not in job2_now.pm2):
                    continue
                if(j + a*job2_now.pt2 >= job2_now.due):
                    emergency = True
                    solved = False
                    if(job2_now.pt2 + j <= job2_now.due):
                        machine[job2_now.ID-1][1] = i
                        completion_time[job2_now.ID -
                                        1][1] = j + job2_now.pt2
                        tmpL = list(accu_machine[k])
                        tmpL[1] += job2_now.pt2
                        accu_machine[k] = tuple(tmpL)
                        solved = True
                        break
            # if the specific machine can't solve the emergency, check others
            if(emergency and solved == False):
                for k, (i, j) in enumerate(accu_machine):
                    if(job2_now.pt2 + j <= job2_now.due):
                        machine[job2_now.ID-1][1] = i
                        completion_time[job2_now.ID -
                                        1][1] = j + job2_now.pt2
                        tmpL = list(accu_machine[k])
                        tmpL[1] += job2_now.pt2
                        accu_machine[k] = tuple(tmpL)
                        solved = True
                        break
            # if no machine can handle it, it's a tardy job
            if(emergency and solved == False):
                tardy.append(job2_now)
                # the removed stage-1 job's machine
                tmpM = machine[job2_now.ID-1][0]
                # the removed stage-1 job's completion time
                tmpC = completion_time[job2_now.ID - 1][0]
                machine[job2_now.ID-1][0] = 0
                completion_time[job2_now.ID - 1][0] = 0
                for i in range(len(jobs)):
                    for j in range(2):
                        # if other jobs are finished after the removed job and on the same machine
                        # minus the process time of removed stage-1
                        if(machine[i][j] == tmpM and completion_time[i][j] > tmpC):
                            completion_time[i][j] -= job2_now.pt1
                # minus accu_time too
                for i in range(len(accu_machine)):
                    if(accu_machine[i][0] == tmpM):
                        tmpL = list(accu_machine[i])
                        tmpL[1] -= job2_now.pt1
                        accu_machine[i] = tuple(tmpL)
            if(emergency):
                stage_2.pop(0)
                for i, j in enumerate(jobs):
                    if j.ID == job2_now.ID:
                        del jobs[i]
                        break
    # tardy = sorted(tardy, key=lambda job: jobs.due)
    # while(tardy):
    #     ptr = 0
    #     jobT_now = tardy[0]
    #     accu_machine = sorted(
    #         accu_machine, key=lambda accu: (accu[1], accu[0]))
    #     while(str(accu_machine[ptr][0]) not in jobT_now.pm1 and ptr < len(accu_machine)):
            # ptr += 1
        # machine[jobT_now.ID-1][0] = accu_machine[ptr][0]
        # completion_time[jobT_now.ID-1][0] = accu_machine[ptr][1] + jobT_now.pt1
    return machine, completion_time

for num in range(5):
    num_ = str(num + 1)
    i, j = heuristic_algorithm(f'data/instance {num_}.csv')
    print(i)
    print(j)
    print("\n")
