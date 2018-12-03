{
  "targets": [
    { 
      "target_name": "read",
      "include_dirs": [ "<(module_root_dir)/server/app/util" ],
      "sources": [ "<(module_root_dir)/server/app/util/read.cpp", "<(module_root_dir)/server/app/util/StdAfx.cpp" ],
      "link_settings": { "libraries": [ "-lVCAIOUSB64" ], "library_dirs" : [ "<(module_root_dir)/server/app/util" ] }
    }
  ]
}