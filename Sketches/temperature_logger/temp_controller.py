import serial
import time
import datetime
import MySQLdb
import threading

arduino = serial.Serial('COM3', 9600)

db = MySQLdb.connect(host = "localhost", user = "Brian", passwd = "#Chaotic5742",
                     db = "arduino_logging")

exit_flag = False

def get_temp(unit):
    if unit in ['f', 'F', 'c', 'C']:
        arduino.write(unit.upper())
        temp = arduino.readline()
        return float(temp)

def insert_temperature_record(db, temp, time):
    cursor = db.cursor()
    insert_query = """INSERT INTO temperatures (reading, unit_id, read_time)
      VALUES ({0}, {1}, '{2}');""".format(temp, 1, time)
    cursor.execute(insert_query)
    db.commit()
    
def log_data(unit = 'F', interval = 15, timeout = -1):
    count = 0
    if timeout > 0:
        while timeout > 0:
            if exit_flag:
                print "EXIT FLAG"
                thread.exit()
            insert_temperature_record(db, get_temp(unit), datetime.datetime.now())
            print "LOG THREAD: Inserted Record #" + str(count)
            count += 1
            timeout -= 1
            time.sleep(interval)
    else:
        while True:
            if exit_flag:
                print "EXIT FLAG"
                thread.exit()
            insert_temperature_record(db, get_temp(unit), datetime.datetime.now())
            print "LOG THREAD: Inserted Record #" + str(count)
            count += 1
            time.sleep(interval)

class data_log_thread (threading.Thread):
    def __init__(self, thread_id, name, counter):
        threading.Thread.__init__(self)
        self.thread_id = thread_id
        self.name = name
        self.counter = counter

    def run(self):
        print "THREAD " + self.name + " STARTING..."
        log_data()
        print "THREAD " + self.name + " EXITING..."


def main():
    temp_log_thread = data_log_thread(1, "TEMP LOG THREAD", 1)
    temp_log_thread.start()

    x = raw_input("Press E to Exit")
    if str(x) == 'E':
        exit_flag = True
    

