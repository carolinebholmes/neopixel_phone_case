__author__ = 'Leandra Irvine'

import requests
import json
import serial

def get_customers():
    #returns list of customer dictionaries
    req = requests.get('http://api.reimaginebanking.com:8080/customers?key=56403237f4a3b0b08b328743c55ba34c')
    json_string = req.text        #contains the result
    response_code = req.status_code #contains the response
    customers = json.loads(json_string) #parsed json
    return customers

def get_balance(account_id):
    req = requests.get('http://api.reimaginebanking.com:8080/accounts/' + account_id + '?key=56403237f4a3b0b08b328743c55ba34c')
    json_string = req.text        #contains the result
    response_code = req.status_code #contains the response
    account = json.loads(json_string) #parsed json
    balance = account.get("balance")
    return balance

def balance_change(account_id):
    no_change = True
    old_balance = get_balance(account_id)
    while (no_change):

        new_balance = get_balance(account_id)
        print("Current Balance:" + repr(new_balance))
        if (new_balance != old_balance):
            change_in_value = new_balance - old_balance
            return (change_in_value)

def get_accounts(customer_id):
    #returns list of customer dictionaries
    req = requests.get('http://api.reimaginebanking.com:8080/customers?key=' + customer_id)
    json_string = req.text        #contains the result
    response_code = req.status_code #contains the response
    accounts = json.loads(json_string) #parsed json
    return accounts

if __name__ == "__main__":
    account_id = "555bed95a520e036e52b26c4"
    start_budget = 600.0
    start_balance = get_balance(account_id)
    budget_left = 600.0
    ser = serial.Serial("/dev/tty.usbmodemfd121", 9600)
    #customers = get_customers()
    #print(customers[0].get("first_name"))
    while(budget_left > 0):
        curr_balance = get_balance(account_id)
        if(curr_balance < 0):
            print("balance below 0")
        balance_diff = balance_change(account_id)
        if (balance_diff < 0):
            print( "blink red")

            budget_left += balance_diff
            if (budget_left >= 0): #no negative percent
                print(budget_left/start_budget)
                ser.write(str(budget_left/start_budget))
            else:
                print("went over budget") #float (went over budget)
                ser.write(str(0.0))
        else:
            #something for savings
            print("positive trasaction: blink blue")
            ser.write(str(1.0))

    ser.close()



