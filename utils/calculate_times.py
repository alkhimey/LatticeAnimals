def T(i):
    if i < 8:
        raise Exception
    return 5.233 * 7 ** (i - 8)

def printable_estimate(secs):
    secs = float(secs)

    if secs / (3600*24) > 1:
        return str(secs / (3600*24)) + " days"
    elif secs / (3600) > 1:
        return str(secs / 3600) + " hours"
    elif secs / (60) > 1:
        return str(secs / 60) + " minutes"
    else:
        return str(secs) + " seconds"


def calculate_times(n, n0, comps, tasks):
    n, n0, comps, tasks = float(n), float(n0), float(comps), float(tasks)

    total_time = (T(n) - T(n0)) / comps + tasks * T(n0)
    print "Total expected time is:", printable_estimate(total_time)
    print "Speedup is:", T(n) / total_time
    task_time = T(n0) + (T(n) - T(n0)) / tasks
    print "Individual task time: ", printable_estimate(task_time)
