#include <iostream>

#include "leveldb/db.h"

auto PutData(leveldb::Status mystatus, leveldb::DB* mydb, std::string key, std::string value) -> bool {
  if (mystatus.ok()) {
    mydb->Put(leveldb::WriteOptions(), key, value);
    return true;
  }
  std::cerr << mystatus.ToString() << std::endl;
  return false;
}

auto DeleteData(leveldb::Status mystatus, leveldb::DB* mydb, std::string key) -> bool {
  if (mystatus.ok()) {
    mydb->Delete(leveldb::WriteOptions(), key);
    return true;
  }
  std::cerr << mystatus.ToString() << std::endl;
  return false;
}

void GetData(leveldb::DB* mydb, std::string key, std::string& value) {
  mydb->Get(leveldb::ReadOptions(), key, &value);
}

int main() {
  // 声明
  leveldb::DB* mydb;
  leveldb::Options myoptions;
  leveldb::Status mystatus;

  // 创建
  myoptions.create_if_missing = true;
  mystatus = leveldb::DB::Open(myoptions, "testdb", &mydb);
  std::string result;
  
  // 对增删查进行测试
  PutData(mystatus, mydb, "nixianjun", "a handsome man");
  GetData(mydb, "nixianjun", result);
  std::cout << "nixianjun : " << result << std::endl;

  PutData(mystatus, mydb, "zhaokangkang", "a beautiful girl");
  GetData(mydb, "zhaokangkang", result);
  std::cout << "zhaokangkang : " << result << std::endl;

  std::cout << "delete nixianjun" << std::endl;
  DeleteData(mystatus, mydb, "nixianjun");
  GetData(mydb, "nixianjun", result);
  std::cout << "nixianjun : " << result << std::endl;
  GetData(mydb, "zhaokangkang", result);
  std::cout << "zhaokangkang : " << result << std::endl;

  std::cout << "delete zhaokangkang" << std::endl;
  DeleteData(mystatus, mydb, "zhaokangkang");
  GetData(mydb, "nixianjun", result);
  std::cout << "nixianjun : " << result << std::endl;
  GetData(mydb, "zhaokangkang", result);
  std::cout << "zhaokangkang : " << result << std::endl;

  std::cout << "put xxx" << std::endl;
  PutData(mystatus, mydb, "xxx", "Others");
  GetData(mydb, "nixianjun", result);
  std::cout << "nixianjun : " << result << std::endl;
  GetData(mydb, "zhaokangkang", result);
  std::cout << "zhaokangkang : " << result << std::endl;

  /*
  nixianjun:a handsome man
  zhaokangkang : a beautiful girl
  delete nixianjun
  nixianjun : a beautiful girl
  zhaokangkang : a beautiful girl
  delete zhaokangkang 
  nixianjun : a beautiful girl
  zhaokangkang : a beautiful girl
  put xxx
  nixianjun : a beautiful girl
  zhaokangkang : a beautiful girl
  */

  std::cout << "------------------------------" << std::endl;
  
  // 测试不用WriteBatch，会出BUG的情况，然而并没有BUG
  PutData(mystatus, mydb, "nixianjun", "a handsome man");
  PutData(mystatus, mydb, "zhaokangkang", "a beautiful girl");
  if (mystatus.ok()) {
    mydb->Delete(leveldb::WriteOptions(), "nixianjun");
    mydb->Put(leveldb::WriteOptions(), "nixianjun", "a funny man");
  }
  GetData(mydb, "nixianjun", result);
  std::cout << "nixianjun : " << result << std::endl;

  // 测试原子批次更新


  // 删除数据库
  mystatus = leveldb::DestroyDB("testdb", myoptions);
  delete mydb;

}