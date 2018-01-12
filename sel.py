# -*- coding: utf8 -*-

from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions

driver = webdriver.Firefox()
driver.get("https://mp.weixin.qq.com")

account_input = driver.find_element_by_id("account")
account_input.clear()
account_input.send_keys("neodreamer@163.com")

pwd_input = driver.find_element_by_id("pwd")
pwd_input.clear()
pwd_input.send_keys("7f7cdb23")

driver.find_element_by_id("loginBt").click()

qr_scan_msg = u'为保障帐号安全，请用微信扫码验证身份'
def mywait(driver):
  return qr_scan_msg in driver.page_source

WebDriverWait(driver, 30).until(mywait)
print "scan page loaded!"

#driver.close()
