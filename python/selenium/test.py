#!/usr/bin/env python
# coding: utf-8

from selenium import webdriver
import os
import time
import bs4

# On montre l'utilisation de selenium en dehors de django.
driver = webdriver.Firefox()

try:
    driver.maximize_window()

    htmlfile = os.path.join(os.path.abspath("."), "index.html")
    url = "file://{}".format(htmlfile)

    def read_value():
        """ Reads and returns the value of the display (from the page source). """
        soup = bs4.BeautifulSoup(driver.page_source, "html.parser")
        value = soup.select("#value")[0]
        return u" ".join(value.stripped_strings)

    driver.get(url)

    # On montre qu'on peut exécuter du javascript depuis selenium :
    driver.execute_script("alert('Hello world !');")
    time.sleep(1)
    webdriver.common.alert.Alert(driver).dismiss()

    # On montre que selenium a accès au javascript de la page (ici, aux services fournis par la libcase.js, chargé dans la page) :
    driver.execute_script("""
    var returned = upper_case("leia");
    $("#value").html(returned);
    """)
    assert(read_value() == "LEIA")
    time.sleep(1)

    # On montre que de base, les execute_script sont indépendants les uns des autres :
    driver.execute_script("""
    var pouet = "POUET1";
    """)
    time.sleep(1)
    driver.execute_script("""
    alert("pouet (1) = " + (typeof pouet === 'undefined' ? "Undefined" : "Defined"));
    """)
    time.sleep(1)
    webdriver.common.alert.Alert(driver).dismiss()


    # On montre qu'en rusant un peu, on peut tout de même partager des données entre les appels à execute_script:
    driver.execute_script("""
    window.pouet = "POUET2";
    """)
    time.sleep(1)
    driver.execute_script("""
    alert("pouet (2) = " + window.pouet);
    """)
    time.sleep(1)
    webdriver.common.alert.Alert(driver).dismiss()




finally:
    time.sleep(1)
    driver.quit()
