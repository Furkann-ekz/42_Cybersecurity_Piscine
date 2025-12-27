import os
import argparse
import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin, urlparse
from termcolor import colored

class  Spider:
	def __init__(self, base_url, max_depth, path):
		self.base_url = base_url
		self.max_depth = max_depth
		self.path = path
		self.visited_urls = set()
		self.downloaded_images = set()

	def get_html():

if (__name__ == "__main__"):
	av = argparse.ArgumentParser(description = "42 Cybersecurity - Spider web crawl.")
	av.add_argument("url", type = str, help = "Target URL to be scanned.")
	av.add_argument("-r", "--recursive", action = "store_true", help = "Scan other links too.")
	av.add_argument("-p", "--path", type = str, default = "/data", help = "Path for downloaded images.")
	av.add_argument("-l", "--level", type = int, default = 5, help = "Depth of the recursion (default: 5)")

	args = av.parse_args()
	
