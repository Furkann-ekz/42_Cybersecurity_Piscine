import os
import argparse
import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin, urlparse
from termcolor import colored

if (__name__ == "__main__"):
	av = argparse.ArgumentParser(description = "42 Cybersecurity - Spider web crawl.")
	av.add_argument("url", type = str, help = "")
