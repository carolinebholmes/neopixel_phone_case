__author__ = 'Leandra Irvine'

import requests
import json

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
    budget = 600
    num_leds = 6
    customers = get_customers()
    #print(customers[0].get("first_name"))
    print(get_balance(account_id))
    print(balance_change(account_id))




