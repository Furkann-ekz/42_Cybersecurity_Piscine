import os
import argparse
import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin, urlparse
from termcolor import colored


class Spider:
    def __init__(self, base_url, path, max_depth):
        self.base_url = base_url
        self.max_depth = max_depth
        self.path = path
        self.visited_urls = set()
        self.downloaded_images = set()
        self.base_domain = urlparse(base_url).netloc
        self.headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) '
                          'AppleWebKit/537.36 (KHTML, like Gecko) '
                          'Chrome/91.0.4472.124 Safari/537.36'
        }

    def get_html(self, base_url):
        try:
            requests_response = requests.get(base_url, timeout=5,
                                             headers=self.headers)
            if (requests_response.status_code != 200):
                print(colored(f"Error: {requests_response.status_code}\n",
                              "red"))
                return (None)
            html = requests_response.text
            return (html)
        except Exception:
            print(colored("Error: get_html function has an error.\n", "red"))
            return (None)

    def download_image(self, img_url):
        try:
            if (not img_url):
                return (None)
            os.makedirs(self.path, exist_ok=True)
            filename = img_url.split("/")[-1].split("?")[0]
            valid_extensions = [".jpg", ".jpeg", ".png", ".gif", ".bmp",
                                ".svg", ".webp", ".tiff"]

            fn = filename
            if (not any(fn.lower().endswith(ext) for ext in valid_extensions)):
                return (None)

            if (len(filename) == 0):
                filename = "unnamed_image.jpg"
            file_path = os.path.join(self.path, filename)

            if (os.path.exists(file_path)):
                print(colored(f"[!] Already exists: {filename}", "yellow"))
                return (None)
            response = requests.get(img_url, headers=self.headers, timeout=5)

            with open(file_path, "wb") as f:
                f.write(response.content)
                print(colored(f"[+] Downloaded: {filename}", "cyan"))

        except Exception:
            print(colored("Image cannot downloading.", "red"))
            return (None)

    def crawl(self, current_url, current_depth):
        try:
            if (current_depth > self.max_depth):
                return (None)
            if (current_url in self.visited_urls):
                return (None)

            print(colored(f"[*] Searching (Level {current_depth}):"
                          f"{current_url}", "green"))

            self.visited_urls.add(current_url)
            html = self.get_html(current_url)

            if (html is None):
                print(colored(f"This link ({current_url}) could not be read.",
                              "red"))
                return (None)

            html_soup = BeautifulSoup(html, "html.parser")

            images = html_soup.find_all("img")
            for img in images:
                src = img.get("src")
                if (src):
                    full_img_url = urljoin(current_url, src)
                    self.download_image(full_img_url)

            links = html_soup.find_all("a")
            for link in links:
                href = link.get("href")
                if (href):
                    full_page_url = urljoin(current_url, href)
                    full_page_url = full_page_url.split("#")[0]

                    domain = urlparse(full_page_url).netloc
                    if (domain == self.base_domain):
                        self.crawl(full_page_url, current_depth + 1)
        except Exception:
            print(colored("Error: crawl function has taken an error.", "red"))


if (__name__ == "__main__"):
    av = argparse.ArgumentParser(description="42 Cybersecurity - "
                                 "Spider web crawl.")
    av.add_argument("url", type=str, help="Target URL to be scanned.")
    av.add_argument("-r", "--recursive", action="store_true",
                    help="Scan other links too.")
    av.add_argument("-p", "--path", type=str, default="./data",
                    help="Path for downloaded images.")
    av.add_argument("-l", "--level", type=int, default=5,
                    help="Depth of the recursion (default: 5)")

    args = av.parse_args()

    if (args.recursive):
        target_depth = args.level
    else:
        target_depth = 0

    print(colored(f"Target  : {args.url}", "magenta"))
    print(colored(f"Path    : {args.path}", "magenta"))
    print(colored(f"Recurse : {args.recursive}", "magenta"))
    print(colored(f"Depth   : {target_depth}", "magenta"))
    print(colored("-" * 40, "magenta"))
    try:
        spider = Spider(args.url, args.path, target_depth)
        spider.crawl(args.url, 0)
    except KeyboardInterrupt:
        print(colored("\n[!] User interrupted the spider. Exiting...",
                      "yellow"))
