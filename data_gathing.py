f = open("login0000.txt")
contexts = f.readlines()
#get context of the access journal file
#print(contexts)

ips = {}#the dictionary of ips
repe = 0
for index in range(len(contexts)):
    line = contexts[index]
    line_phrases = line.split(" ")
    #get parts of the journal line

    ip = line_phrases[0]#get ips
    if ip in ips.keys():#avoid repetitions
        repe = repe + 1#count the repetitions
        ip = ip + ";time" + str(index)
    #print(ip)

    timerecord = ""
    for phrase in line_phrases:
        for digit in range(len(phrase)):
            if phrase[digit] == "[":#to find the position of time record
                timerecord = phrase
    #make the phrase with "[" be found as timerecord
    ips[ip] = timerecord#key:ip value:time

pv = len(ips)
uv = len(ips) - repe
print(pv)
print(uv)
print(ips)

