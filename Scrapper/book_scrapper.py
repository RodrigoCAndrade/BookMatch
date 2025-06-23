import requests
import json

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
    return {
        "title": book.get('title', ''),
        "author": ', '.join(book.get('authors', [])),
        "date": book.get('publishedDate', ''),
        "publisher": book.get('publisher', ''),
        "description": book.get('description', ''),
        "genre": ', '.join(book.get('categories', [])),
        "tags": ''
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
    return {
        "title": book.get('title', ''),
        "author": authors,
        "date": book.get('publish_date', ''),
        "publisher": ', '.join([p['name'] for p in book.get('publishers', [])]),
        "description": description,
        "genre": genres,
        "tags": ''
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
    isbns = set(line.strip() for line in f if line.strip())

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
