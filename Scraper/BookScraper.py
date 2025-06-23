import requests
import json
import datetime
from dateutil import parser

def parse_date_to_iso(date_str):
    # Converte para ISO 8601 (YYYY-MM-DD)
    try:
        # Google Books: 'YYYY', 'YYYY-MM', 'YYYY-MM-DD'
        if len(date_str) == 4:
            return f"{date_str}-01-01T00:00:00"
        elif len(date_str) == 7:
            return f"{date_str}-01T00:00:00"
        elif len(date_str) == 10:
            return f"{date_str}T00:00:00"
        # OpenLibrary: 'Month DD, YYYY'
        return parser.parse(date_str).isoformat()
    except Exception:
        return date_str

def fetch_google_books(isbn):
    url = f"https://www.googleapis.com/books/v1/volumes?q=isbn:{isbn}"
    response = requests.get(url)
    if response.status_code != 200:
        return None
    data = response.json()
    items = data.get('items', [])
    if not items:
        return None
    book = items[0]['volumeInfo']
    raw_date = book.get('publishedDate', '')
    iso_date = raw_date
    if raw_date:
        try:
            iso_date = parser.parse(raw_date).date().isoformat()
        except Exception:
            iso_date = raw_date
    return {
        "title": book.get('title', ''),
        "author": ', '.join(book.get('authors', [])),
        "date": iso_date,
        "createdDate": datetime.datetime.now().isoformat(),
        "publisher": book.get('publisher', ''),
        "description": book.get('description', ''),
        "genre": ', '.join(book.get('categories', [])),
        "tags": '',
    }

def fetch_openlibrary(isbn):
    url = f"https://openlibrary.org/api/books?bibkeys=ISBN:{isbn}&format=json&jscmd=data"
    response = requests.get(url)
    if response.status_code != 200:
        return None
    data = response.json()
    key = f"ISBN:{isbn}"
    if key not in data:
        return None
    book = data[key]
    authors = ', '.join([a['name'] for a in book.get('authors', [])])
    genres = ', '.join([s['name'] for s in book.get('subjects', [])]) if 'subjects' in book else ''
    description = ''
    if 'description' in book:
        if isinstance(book['description'], dict):
            description = book['description'].get('value', '')
        else:
            description = book['description']
    raw_date = book.get('publish_date', '')
    iso_date = raw_date
    if raw_date:
        try:
            iso_date = parser.parse(raw_date).date().isoformat()
        except Exception:
            iso_date = raw_date
    return {
        "title": book.get('title', ''),
        "author": authors,
        "date": iso_date,
        "createdDate": datetime.datetime.now().isoformat(),
        "publisher": ', '.join([p['name'] for p in book.get('publishers', [])]),
        "description": description,
        "genre": genres,
        "tags": '',
    }

def fetch_book_data(isbn):
    data = fetch_google_books(isbn)
    if data and data["title"]:
        return data
    data = fetch_openlibrary(isbn)
    if data and data["title"]:
        return data

with open('isbns.txt', 'a+', encoding='utf-8') as f:
    f.seek(0)
    isbns = set(
        line.strip() for line in f
        if line.strip() and not line.lstrip().startswith(('//', '#', '!'))
    )

books = {}
for isbn in isbns:
    print(f"Consultando ISBN: {isbn}")
    data = fetch_book_data(isbn.replace("-", ""))
    if data:
        books[isbn] = data
    else:
        print(f"Não encontrado: {isbn}")

with open('books.json', 'w', encoding='utf-8') as f:
    json.dump(books, f, ensure_ascii=False, indent=4)

print("Finalizado! Dados salvos em books.json.")
