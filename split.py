import sys, time, datetime



def splitByHour(filename_in, start_hour, nhours=1):
    '''
        Split a large file into several smaller files according to their pvid. First seven digits of pvid represnet impression time.
        format of start_hour: 201408060800
    '''

    extension = finput.split('.')[1]

    boundary = []
    obj_date = datetime.datetime.strptime(start_date, "%Y%m%d%H%M")
    for h in xrange(nhours):
        new_date =  obj_date + datetime.timedelta(days=(-h))
        filename = new_date.strftime('%Y%m%d%H%M') + '.' + extension
        timestamp = str(time.mktime(new_date.timetuple())+3600)[3:10]
        boundary.append( (timestamp, filename) )

    hour = 0
    fin = open(filename_in, 'r')

    MAXSEC, filename_out = boundary[hour]
    fout = open(filename_out, 'w')
    for line in fin:
        unixtime = line[0:7]
        if unixtime < MAXSEC:
            print >>fout, line.strip()
        else:
            hour += 1
            if hour >= nhours:
                print 'exceed maximun hours: %d' %(hour)
                break
            fout.close()
            print "*** Done with file %s." %(filename_out)
            MAXSEC, filename_out = boundary[hour]
            fout = open(filename_out, 'w')
    fout.close()
    print "*** Done with file %s." %(filename_out)


def splitByDay(filename_in, start_date, ndays=1):     
    '''
        Split a large file into several smaller files according to their pvid. First seven digits of pvid represnet impression time.
        format of start_date: 20140806
    '''

    extension = finput.split('.')[1]

    boundary = []
    obj_date = datetime.datetime.strptime(start_date, "%Y%m%d")
    for day in xrange(ndays):
        new_date =  obj_date + datetime.timedelta(days=(-day))
        filename = new_date.strftime('%Y%m%d') + '.' + extension
        timestamp = str(time.mktime(new_date.timetuple())+3600*24)[3:10]
        boundary.append( (timestamp, filename) )

    day = 0
    fin = open(filename_in, 'r')

    MAXSEC, filename_out = boundary[day]
    fout = open(filename_out, 'w')
    for line in fin:
        unixtime = line[0:7]
        if unixtime < MAXSEC:
            print >>fout, line.strip()
        else:
            day += 1
            if day >= days:
                print 'exceed maximun hours: %d' %(day)
                break
            fout.close()
            print "*** Done with file %s." %(filename_out)
            MAXSEC, filename_out = boundary[day]
            fout = open(filename_out, 'w')
    fout.close()
    print "*** Done with file %s." %(filename_out)


