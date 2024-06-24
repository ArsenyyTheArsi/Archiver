# Archiever

Для компиляции в gcc использовать:

gcc main.c functions/create_zip.c functions/extract_zip.c functions/list_archive.c functions/add_files_to_archive.c functions/delete_files_from_archive.c -o Archiver -Ifunctions 

# Функции

- "-c" или "--create" – создать архив.
- "-x" или "--extract" – извлечь архив.
- "-l" или "--list" – просмотреть содержимое архива.
- "-a" или "--add" – добавить файлы в архив.
- "-d" или "--delete" – удалить файлы из архива.
- "-f" или "--file" – указать путь к архиву или к файлам для архивации.
- "-o" или "--output" – путь для сохранения архива или извлеченных файлов.
- "-v" или "--verbose" – режим с детальным выводом процесса работы.
- "-s" или "--size" – указание уровня сжатия (только для архивации).


