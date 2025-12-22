import requests, argparse, os
from bs4 import BeautifulSoup
from termcolor import colored
from urllib.parse import urljoin, urlparse

class Spider:
	def __init__(self, base_url, save_path, max_depth = 5):
		self.base_url = base_url
		self.save_path = save_path
		self.max_depth = max_depth
		self.visited_urls = set()
		self.downloaded_images = set()
		self.base_domain = urlparse(base_url).netloc

		self.headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) '
                          'AppleWebKit/537.36 (KHTML, like Gecko) '
                          'Chrome/91.0.4472.124 Safari/537.36'
        }



	def	get_html(self, url):
		try:
			response = requests.get(url, headers = self.headers, timeout = 5)
			if (response.status_code != 200):
				print(colored(f"Request error code: {response.status_code}.", "red"))
				return None
			return response.text
		except Exception:
			return None



	def download_image(self, img_url):
		if (img_url in self.downloaded_images):
			return
		self.downloaded_images.add(img_url)

		try:
			os.makedirs(self.save_path, exist_ok = True)

			filename = img_url.split("/")[-1].split("?")[0]
			if (len(filename) == 0:)
				filename = "unnamed_image.jpg"
			file_path = os.path.join(self.save_path, filename)

			if (os.path.exists(file_path)):
				print(colored(f"[!] Already exists: {filename}", "yellow"))
				return

			response = requests.get(img_url, headers = self.headers, timeout = 5)

			with open(file_path, "wb") as f:
				f.write(response.content)
				print(colored(f"[+] Downloaded: {filename}", "cyan"))

		except Exception:
			print(colored("Image cannot downloading.", "red"))
			return



	def crawl(self, current_url, current_depth):
		if (current_depth > self.max_depth):
			return

		if (current_url in self.visited_urls):
			return

		print(colored(f"[*] Searching (Level {current_depth}): {current_url}", "green"))

		self.visited_urls.add(current_url)
		html = self.get_html(current_url)

		if (not html):
			print(colored(f"This link ({current_url}) could not be read.", "red"))
			return
		
		soup = BeautifulSoup(html, 'html.parser')

		images = soup.find_all('img')
		for img in images:
			src = img.get("src")
			if src:
				full_img_url = urljoin(current_url, src)
				self.download_image(full_img_url)

		links = soup.find_all('a')
		for link in links:
			href = link.get('href')
			if href:
				full_page_url = urljoin(current_url, href)

				full_page_url = full_page_url.split("#")[0]

				link_domain = urlparse(full_page_url).netloc
				if urlparse(full_page_url).netloc == self.base_domain:
					self.crawl(full_page_url, current_depth + 1)



if __name__ == "__main__":
	parser = argparse.ArgumentParser(description = "Spider - A web crawler for 42 Cybersecurity")

	parser.add_argument("url", help = "The URL to start crawling")
	parser.add_argument("-r", "--recursive", action = "store_true", help = "Enable recursive download")
	parser.add_argument("-l", "--level", type = int, default = 5, help = "Depth of the recursion (default: 5)")
	parser.add_argument("-p", "--path", type = str, default = "./data", help = "Path to save images")

	args = parser.parse_args()

	target_depth = args.level if args.recursive else 0

	print(colored(f"Target  : {args.url}", "magenta"))
	print(colored(f"Path    : {args.path}", "magenta"))
	print(colored(f"Recurse : {args.recursive}", "magenta"))
	print(colored(f"Depth   : {target_depth}", "magenta"))
	print(colored("-" * 40, "magenta"))

	try:
		spider = Spider(args.url, args.path, target_depth)
		spider.crawl(args.url, 0)
	except KeyboardInterrupt:
		print(colored("\n[!] User interrupted the spider. Exiting...", "yellow"))