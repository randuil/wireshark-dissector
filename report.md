# План выполнения индивидуального задания

## Задание 1.
1. Скачать исходники Wireshark
2. Установть необходимые зависимости
3. Скомпилировать Wireshark
4. Настроить права файлов для корректной работы
5. Определить тестовый протокол
6. Написать тестовые программы для протокола
7. Написать плагин-диссектор
8. Убедиться в работоспособности диссектора

# Теоретические данные
**Wireshark** - широко известный, мультиплатформенный анализатор сетевых протоколов с открытым исходным кодом `https://github.com/wireshark/wireshark`. Позволяет анализировать данные как в режиме реального времени, так и собраные заранее.

**Диссектор** - Дисектор (от лат. dissect, рассекать/расчленять)- программа, которая представляет данные пакета в удобном для человека виде.

# Выполнение индивидуального задания
## Задание 1
## Получение исходников Wireshark
Исходные файлы лежат в публичном репозитории по адресу `https://github.com/wireshark/wireshark`

1. Создание директории `wireshark-src`
   
   ```bash
   $ mkdir wireshark-src
   $ cd wireshark-src
   ```
2. Клонирование репозитория
	```bash
	$ git clone https://gitlab.com/wireshark/wireshark.git
	```

## Установка необходимых зависимостей
1. Список зависимостей:
	```
	libglib2.0-dev
    libgcrypt20-dev
    libc-ares-dev
    flex
    bison
    qttools5-dev
    qttools5-dev-tools
    libqt5svg5-dev
    qtmultimedia5-dev
    qttools5-dev
    qttools5-dev-tools
    libqt5svg5-dev
    qtmultimedia5-dev
	```
2. Установка зависимостей.
	```bash
	$ sudo apt install libglib2.0-dev libgcrypt20-dev libc-ares-dev flex bison qttools5-dev qttools5-dev-tools libqt5svg5-dev qtmultimedia5-dev qttools5-dev qttools5-dev-tools libqt5svg5-dev qtmultimedia5-dev
	```
## Сборка wireshark
1. Создание директории `Foo-BUILD`. В этой директории будут находиться файлы после сборки
	```bash 
	$ mkdir Foo-BUILD
   	$ cd Foo-BUILD
   ```
2. Использование `CMake` для создания файлов `make`
	```bash
	$ cmake ../wireshark
	```
	Частичный вывод программы:
	```
	---SNIP---
	-- The following REQUIRED packages have been found:
	 * GLIB2 (required version >= 2.38.0)   
	 * GTHREAD2  
	 * GCRYPT (required version >= 1.5.0)   
	 * CARES (required version >= 1.5.0), Library for asynchronous DNS requests, <https://c-ares.haxx.se/>   
	   DNS name resolution for captures  
	 * LEX   
	 * Perl  
	 * Python3 (required version >= 3.4)   
	 * M     
	 * Qt5Core   
	 * Qt5LinguistTools  
	 * Qt5Network (required version >= 5.15.2)   
	 * Qt5Gui (required version >= 5.15.2) 
	 * Qt5Multimedia     
	 * Qt5PrintSupport   
	 * Qt5Widgets    
	 * POD
	 
	-- Configuring done
	-- Generating done
	-- Build files have been written to: /home/randuil/Documents/Work/Wireshark/wireshark-src/Foo-BUILD
	```
2. Использование `make` для сборки wireshark
	```bash
	$ make
	```
	Частичный вывод программы:
	```
	[ 93%] Built target irda
    [ 94%] Built target mate
    [ 94%] Built target opcua
    [ 95%] Built target profinet
    [ 95%] Built target stats_tree
    [ 95%] Built target transum
    [ 95%] Built target unistim
    [ 97%] Built target wimax
    [ 97%] Built target wimaxasncp
    [ 98%] Built target wimaxmacphy
    [ 98%] Built target usbdump
    [ 98%] Built target g711
    [ 98%] Built target l16mono
    [ 98%] Generating plugins/foo/plugin.c
    Scanning dependencies of target foo
    [ 98%] Building C object plugins/foo/CMakeFiles/foo.dir/plugin.c.o
    [ 98%] Building C object plugins/foo/CMakeFiles/foo.dir/packet-foo.c.o
    [ 98%] Linking C shared module ../../run/plugins/3.5/epan/foo.so
    [ 98%] Built target foo
    [100%] Built target fuzzshark
	```
## Настройка прав
1. Создание группы `wireshark` с помощью команды `groupadd`
	```bash
	$ sudo groupadd -s wireshark
	```
2. Добавление текущего пользователя к этой группе с помощью команды `gpasswd`
	```bash
	$ sudo gpasswd -a $USER wireshark
	```
3. Ограничение доступа для других пользователей с помощью команды `chmod`
	```bash
	$ sudo chmod o-rx run/dumpcap
	```
4. Результат
	```bash
	$ ls -l
   -rwxr-x--- 1 randuil wireshark 439120 июл 23 05:08 run/dumpcap
	```
5. Настройка `capabilities` (если поддерживается ядром) для `dumpcap`, что позволит непривелегированному процессу  перехватывать траффик на интерфейсах

   ```bash
   $ sudo setcap cap_net_raw,cap_net_admin+eip run/dumpcap
   ```
6. Необходимо совершить повторный вход в систему для приминения изменений

## Тестовый запуск
1. Запуск исполняемого файла из директории `run`

   ```bash
   cd run && ./wireshark
   ```

![image-20210723055143707](/home/randuil/.config/Typora/typora-user-images/image-20210723055143707.png)

2. Пример перехвачено HTTP трафика

   ![риc1](/home/randuil/.config/Typora/typora-user-images/image-20210723055919167.png)
   
   рис 1.

## Описание тестового протокола
Протокол позволяет отправлять сообщения.
Общение происходит под протоколом UDP, порт сервера: `1234`
1. Структура пакета:
	- 1 байт — версия протокола
   
    - 1 байт — тип пакета: 
   | Значение в HEX |    тип    |
   | :------------: | :-------: |
   |      0x01      |   begin   |
   |      0x02      | terminate |
   |      0x03      |   data    |

    - 1 байт — всевозможные флаги:
   | Значение в HEX | Флаг  	|
   | :------------: | :-------: |
   |      0x01      |   start 	|
   |      0x02      |    end  	|
   |      0x03      |    data  	|
   |      0x04      |    info  	|
   
      
   
    - 1 байт — какая-то булева переменная
   
    - 4 байт — длина полезной нагрузки
   
    - От 0 до 200 байт — полезная нагрузка

## Написание программ, осуществляющих общение по тестовому протоколу

Общение осуществляется между программами, написанными на языке `python` и запущенными на 2-х разных устройствах в пределах локальной сети. У серверного устройства ip адрес `192.168.0.2`. У клиентского устройства адрес `192.168.0.89`

1. Серверная программа
	Код в приложении 1.1

Алгоритм работы серверной програмы:

	1. Cоздаёт UDP сокет  на порту `1234`
	2. Ждёт подключения от клиента
	3. Получает данные
	4. Выводит информацию с использованием цвето-графических обозначений

2. Клиентская програма

   

Алгоритм работы клиентской програмы:

	1. Создаёт UDP сокет
	2. Если програма запущена без аргументов, то полезной нагрузкой становится заранее подготовленное сообщение. Иначе, первый аргумент кодируется для отправки.
	3. Формирует пакет
	4. Отправляет пакет

## Плагин-диссектор

1. Нобходимо разместить файл с исходным кодом диссектора и файл конфигурации CMAKE  в папке, соответствующей названию протокола(в данном случае `foo`) находящейся в папке `wireshark/plugins`.  Пути получается следующими: `wireshak/plugins/foo/packet-foo.c` и `wireshark/plugins/foo/CMakeLists.txt`

2. Необходимо создать файл `/wireshark/CMakeListsCustom.txt` 

   Cодержание файлов находится в прил 2.1,2.2 и 2.3

## Проверка работоспособности диссектора





Для запуска wireshark необходимо повторить шаги сборки и настройки прав.

1. Запуск сервера

   ```bash
   $ python server.py
   ```

2. Запуск клиента с различными данными

```bash
$ python client.py
$ python client.py 
'SomeLongLongStringSomeLongLongStringSomeLongLongStringSomeLongLongStringSomeLongLongStringSomeLongLongStringSomeLongLongStringSomeLongLongString'
$ python client.py 'Русский текст с пробелами'
$ python client.py 'This is a test 12345 54321'
```

3. Вывод сервера

![image-20210803161610016](/home/randuil/.config/Typora/typora-user-images/image-20210803161610016.png)

рис 2.

4. Пакеты, перехваченые Wireshark

   ![image-20210803162032505](/home/randuil/.config/Typora/typora-user-images/image-20210803162032505.png)
   
   рис 3.
   
   
# Приложения
1.1 `server.py` 
```python	
   import socket
   
   def pretty_print(text, colour):
       print("\033[38;5;"+str(colour)+"m"+text+"\033[0;0m", end="") ## Andsi colours
   
   server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   server.bind(('192.168.0.2', 1234))
   
   while True:
       data, address = server.recvfrom(2024)
       print(f'connect from: {address}')
       header = data[:8].hex()
       print('\tHeader:', end="")
       pretty_print(header[:2],220)
       pretty_print(header[2:4],225)
       pretty_print(header[4:6],117)
       pretty_print(header[6:8],196)
       pretty_print(header[8:16],141)
       pretty_print(header[16:],172)
       print(f"\n\tData: \033[38;5;123m {data[8:].hex()}\033[0;0m\n")
```

1.2 `client.py`
```python
   import socket
   import sys
   
   client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   
   
   version = b"\x03"
   # 01 - begin 02 - terminate 03 - data
   packetType = b"\x03"
   
   # 01 - start 02 -end  04 - data 08 - info
   packetFlags = b"\x08"
   
   packetBool = b"\x01"
   
   # must be less than 200 bytes
   if len(sys.argv) ==1:
       payload = b"Hello there!"
   else:
       payload = sys.argv[1].encode()
   
   
   payloadLength = (len(payload)).to_bytes(4,'big')
   
   message = version + packetType + packetFlags + packetBool +  payloadLength +  payload  
   
   
   client.sendto(message, ('192.168.0.2',1234))
   client.close()
   
```

2.1 `wireshak/plugins/foo/packet-foo.c`
```c
   #include "config.h"
#include <epan/packet.h>

//Protocol port
#define FOO_PORT 1234

//Flags
#define FOO_START_FLAG      0x01
#define FOO_END_FLAG        0x02
#define FOO_PRIORITY_FLAG   0x04
#define FOO_INFO_FLAG   	0x08


//Protocol handelrs, will be set when the dissector is registered within the main program
static int proto_foo = -1;
static int hf_foo_pdu_type = -1;
static int hf_foo_hdr_version = -1;

//Flags handlers
static int hf_foo_flags = -1;
static int hf_foo_startflag = -1;
static int hf_foo_endflag = -1;
static int hf_foo_infoflag = -1;
static int hf_foo_priorityflag = -1;

//Boolean handler
static int hf_foo_bool = -1;

//Payload length handler
static int hf_foo_pl_len = -1;

//Payload handler
static int hf_foo_payload = -1;

//End of protocol subtree handler
static gint ett_foo = -1;

//packet types
static const value_string packettypenames[] = {
    { 1, "Initialise" },
    { 2, "Terminate" },
    { 3, "Data" },
    { 0, NULL }
};

static int
dissect_foo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_)
{
	gint offset = 0;
	/* Get packet type */
	guint8 packet_type = tvb_get_guint8(tvb, 1);
	
	/*Set protocol column to FOO*/
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "FOO");
    
    /* Clear the info column */
    col_clear(pinfo->cinfo,COL_INFO);
    
    /* Add packet type to info column */
    col_add_fstr(pinfo->cinfo, COL_INFO, "Type %s",val_to_str(packet_type, packettypenames, "Unknown (0x%02x)"));
    
    /* Add "FOO" subtree*/
	proto_item *ti = proto_tree_add_item(tree, proto_foo, tvb, 0, -1, ENC_NA);
	/* Add packet type to "FOO" subtree */
	proto_item_append_text(ti, ", Type %s",
        val_to_str(packet_type, packettypenames, "Unknown (0x%02x)"));
    
    /* Add "FOO" subtree to "main" tree */
    proto_tree *foo_tree = proto_item_add_subtree(ti, ett_foo);
    
    /* Add "version" field to "main" tree */
    proto_tree_add_item(foo_tree, hf_foo_hdr_version, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset += 1;
    
    /* Add packet "type" field to "main" tree */
    proto_tree_add_item(foo_tree, hf_foo_pdu_type, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset += 1;
    
    /* Add packet "flags" field to "main" tree*/
    static int* const bits[] = {
            &hf_foo_startflag,
            &hf_foo_endflag,
            &hf_foo_priorityflag,
            &hf_foo_infoflag,
            NULL
        };
    proto_tree_add_bitmask(foo_tree, tvb, offset, hf_foo_flags, ett_foo, bits, ENC_BIG_ENDIAN);
	offset += 1;
	
	/* Add packet "boolean" field to "main" tree*/
	proto_tree_add_item(foo_tree, hf_foo_bool, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
    
    /* Add packet "payload length" field to "main" tree*/
    proto_tree_add_item(foo_tree, hf_foo_pl_len, tvb, offset, 4, ENC_BIG_ENDIAN);
    offset += 4;
    
    /* Add packet "payload" field to "main" tree*/
    proto_tree_add_item(foo_tree, hf_foo_payload, tvb, offset, -1, ENC_BIG_ENDIAN);
    
    return tvb_captured_length(tvb);
}

//Used to register protocol in wireshark
void
proto_register_foo(void)
{
    static hf_register_info hf[] = {
        { &hf_foo_pdu_type,						//The node’s index,
            { "FOO Packet Type", "foo.type",    //The item’s label, The item’s abbreviated name
            FT_UINT8, BASE_DEC,					//The iems's type, print as decimal number
            NULL, 0x0,
            NULL, HFILL }
        },
        { &hf_foo_hdr_version, 
			{ "FOO Version", "foo.version",
			FT_UINT8, BASE_DEC,
			NULL, 0x0,
			NULL, HFILL}
		},
		{ &hf_foo_flags,
			{"FOO FLAGS", "foo.flags",
			FT_UINT8, BASE_HEX,
			NULL, 0x0,
			NULL, HFILL}
		},
		{ &hf_foo_startflag,
            { "FOO Start Flags", "foo.flags.start",
            FT_BOOLEAN, 8,
            NULL, FOO_START_FLAG,
            NULL, HFILL }
        },
        { &hf_foo_endflag,
            { "FOO End Flags", "foo.flags.end",
            FT_BOOLEAN, 8,
            NULL, FOO_END_FLAG,
            NULL, HFILL }
        },
        { &hf_foo_priorityflag,
            { "FOO Priority Flags", "foo.flags.priority",
            FT_BOOLEAN, 8,
            NULL, FOO_PRIORITY_FLAG,
            NULL, HFILL }
        },
        { &hf_foo_infoflag,
            { "FOO Info Flags", "foo.flags.info",
            FT_BOOLEAN, 8,
            NULL, FOO_INFO_FLAG,
            NULL, HFILL }
        },
         { &hf_foo_bool,
            { "FOO Header Boolean", "foo.hdr.bool",
            FT_BOOLEAN, BASE_NONE,
            NULL, 0x0,
            NULL, HFILL }
        },
        { &hf_foo_pl_len,
            { "FOO Payload Length", "foo.pl_len",
            FT_UINT8, BASE_DEC,
            NULL, 0x0,
            NULL, HFILL }
        },
		{ &hf_foo_payload,
            { "FOO Payload", "foo.payload",
            FT_STRING, BASE_NONE,
            NULL, 0x0,
            NULL, HFILL }
        }
       
    };

    /* Setup protocol subtree array */
    static gint *ett[] = {
        &ett_foo
    };

    proto_foo = proto_register_protocol (
        "FOO Protocol", /* name       */
        "FOO",          /* short_name */
        "foo"           /* filter_name*/
        );

    proto_register_field_array(proto_foo, hf, array_length(hf));
    proto_register_subtree_array(ett, array_length(ett));
}


//handoff routine associates a protocol handler with the protocol’s traffic
void
proto_reg_handoff_foo(void)
{
    static dissector_handle_t foo_handle;

    foo_handle = create_dissector_handle(dissect_foo, proto_foo);
    dissector_add_uint("udp.port", FOO_PORT, foo_handle);
}
```

2.2 `wireshark/plugins/foo/CMakeLists.txt`
```cmake
    # CMakeLists.txt
    include(WiresharkPlugin)

    # Plugin name and version info (major minor micro extra)
    set_module_info(foo 0 0 4 0)

    set(DISSECTOR_SRC
    packet-foo.c
    )

    set(PLUGIN_FILES
    plugin.c
    ${DISSECTOR_SRC}
    )

    set_source_files_properties(
    ${PLUGIN_FILES}
    PROPERTIES
    COMPILE_FLAGS "${WERROR_COMMON_FLAGS}"
    )

    register_plugin_files(plugin.c
    plugin
    ${DISSECTOR_SRC}
    )

    add_plugin_library(foo epan)

    target_link_libraries(foo epan)

    install_plugin(foo epan)

    file(GLOB DISSECTOR_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
    CHECKAPI(
    NAME
    foo
    SWITCHES
    --group dissectors-prohibited
    --group dissectors-restricted
    SOURCES
    ${DISSECTOR_SRC}
    ${DISSECTOR_HEADERS}
    )

```

2.3  `wireshark/CMakeListsCustom.txt`
```cmake
# CMakeListsCustom.txt
#
# Wireshark - Network traffic analyzer
# By Gerald Combs <gerald@wireshark.org>
# Copyright 1998 Gerald Combs
#
# SPDX-License-Identifier: GPL-2.0-or-later
#
# Add a list of your custom plugins SRC dir here including the path
#

# Fail CMake stage if any of these plugins are missing from source tree
set(CUSTOM_PLUGIN_SRC_DIR
#	private_plugins/foo
# or
#	plugins/epan/foo
)

# Do not fail CMake stage if any of the optional plugins are missing from source tree
set(_OPTIONAL_CUSTOM_PLUGIN_SRC_DIR
	plugins/foo
)

foreach(  _plugin_dir ${_OPTIONAL_CUSTOM_PLUGIN_SRC_DIR} )
	if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_plugin_dir}/CMakeLists.txt )
		list( APPEND CUSTOM_PLUGIN_SRC_DIR ${_plugin_dir} )
	else()
		message( WARNING "Custom plugins: No ${_plugin_dir}/CMakeLists.txt file found - ignoring" )
	endif()
endforeach()

#
# Editor modelines  -  https://www.wireshark.org/tools/modelines.html
#
# Local variables:
# c-basic-offset: 8
# tab-width: 8
# indent-tabs-mode: t
# End:
#
# vi: set shiftwidth=8 tabstop=8 noexpandtab:
# :indentSize=8:tabSize=8:noTabs=false:
#
```



# Вывод

В ходе работы был написан и протестирован плаги-диссектор для малоизвестного протокола, были написаны клиентская и серверная тестирующие програмы. Также были изучены правила и принципы написания диссекторов. 



# Список  использованых источников

1. https://www.wireshark.org/docs/wsdg_html_chunked/ChDissectAdd.html
2. https://docs.python.org/3/library/socket.html