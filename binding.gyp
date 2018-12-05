{
  "targets": [
    { 
      "target_name": "getCode",
      "include_dirs": [ "<(module_root_dir)/server/app/util/cpp" ],
      "sources": [ "<(module_root_dir)/server/app/util/cpp/getCode.cpp", "<(module_root_dir)/server/app/util/cpp/StdAfx.cpp" ],
      "link_settings": { "libraries": [ "-lVCAIOUSB64" ], "library_dirs" : [ "<(module_root_dir)/server/app/util/cpp" ] }
    },
    { 
      "target_name": "stepCode",
      "include_dirs": [ "<(module_root_dir)/server/app/util/cpp" ],
      "sources": [ "<(module_root_dir)/server/app/util/cpp/stepCode.cpp", "<(module_root_dir)/server/app/util/cpp/StdAfx.cpp" ],
      "link_settings": { "libraries": [ "-lVCAIOUSB64" ], "library_dirs" : [ "<(module_root_dir)/server/app/util/cpp" ] }
    },
    { 
      "target_name": "saveCode",
      "include_dirs": [ "<(module_root_dir)/server/app/util/cpp" ],
      "sources": [ "<(module_root_dir)/server/app/util/cpp/saveCode.cpp", "<(module_root_dir)/server/app/util/cpp/StdAfx.cpp" ],
      "link_settings": { "libraries": [ "-lVCAIOUSB64" ], "library_dirs" : [ "<(module_root_dir)/server/app/util/cpp" ] }
    },
    { 
      "target_name": "setCode",
      "include_dirs": [ "<(module_root_dir)/server/app/util/cpp" ],
      "sources": [ "<(module_root_dir)/server/app/util/cpp/setCode.cpp", "<(module_root_dir)/server/app/util/cpp/StdAfx.cpp" ],
      "link_settings": { "libraries": [ "-lVCAIOUSB64" ], "library_dirs" : [ "<(module_root_dir)/server/app/util/cpp" ] }
    },
    { 
      "target_name": "configure",
      "include_dirs": [ "<(module_root_dir)/server/app/util/cpp" ],
      "sources": [ "<(module_root_dir)/server/app/util/cpp/configure.cpp", "<(module_root_dir)/server/app/util/cpp/StdAfx.cpp" ],
      "link_settings": { "libraries": [ "-lVCAIOUSB64" ], "library_dirs" : [ "<(module_root_dir)/server/app/util/cpp" ] }
    }
  ]
}