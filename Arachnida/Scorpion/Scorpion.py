import sys
import os
import time
from PIL import Image
from PIL.ExifTags import TAGS

def get_basic_info(file_path):
	print(f"--- Temel Bilgiler: {file_path} ---")
	path_size = os.path.getsize(file_path)
	img_time = os.path.getctime(file_path)
	
	# İYİLEŞTİRME: Byte'ı KB'ye, Timestamp'i Tarihe çevirelim
	print(f"Boyut          : {path_size} bytes")
	print(f"Oluşturulma    : {time.ctime(img_time)}") # time.ctime okunabilir yapar

def convert_to_degrees(value):
	"""
	(Derece, Dakika, Saniye) formatındaki GPS verisini
	Ondalık formata (Google Maps) çevirir.
	"""
	d = float(value[0])
	m = float(value[1])
	s = float(value[2])
	return d + (m / 60.0) + (s / 3600.0)

def get_exif_data(image):
    print("--- EXIF Verileri ---")
    
    try:
        img_data = image._getexif()
    except Exception:
        print("EXIF verisi okunamadı.")
        return

    if img_data is None:
        print("EXIF verisi yok (None).")
        return

    for tag_id, value in img_data.items():
        
        tag_name = TAGS.get(tag_id, tag_id)

        # --- GPS KONTROL BLOĞU ---
        if tag_name == "GPSInfo":
            gps_data = value

            lat_ref = gps_data.get(1)
            lat_val = gps_data.get(2)
            lon_ref = gps_data.get(3)
            lon_val = gps_data.get(4)

            # DÜZELTME 1: 'l' yerine 'lon_val'
            if lat_val and lon_val and lat_ref and lon_ref:
                lat = convert_to_degrees(lat_val)
                lon = convert_to_degrees(lon_val)

                if lat_ref == "S":
                    lat = -lat
                if lon_ref == "W":
                    lon = -lon
                
                # DÜZELTME 2: 'lot' yerine 'lon'
                print(f"{'GPS Coordinates':25} : {lat}, {lon}")
                
                # DÜZELTME 3: Google Maps link formatını düzelttim
                print(f"{'Google Maps':25} : https://www.google.com/maps?q={lat},{lon}")
                continue
        # -------------------------

        if isinstance(value, bytes):
            value = value.decode(errors="replace")[:50]

        print(f"{tag_name:25} : {value}")

def scorpion(file_path):
	try:
		get_basic_info(file_path)

		img = Image.open(file_path)
		
		print(f"Format         : {img.format}")
		print(f"Çözünürlük     : {img.size}")
		print(f"Renk Modu      : {img.mode}")

		get_exif_data(img)
		
		print("="*40)

	except Exception as e:
		print(f"Hata: {file_path} analiz edilemedi. Sebebi: {e}")

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Kullanım: python3 scorpion.py <dosya1> <dosya2> ...")
	else:
		for file in sys.argv[1:]:
			scorpion(file)