//------------------------------GPLv3------------------------------------//
//
//  This file is part of SumpMonitor.
//
//  SumpMonitor is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SumpMonitor is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SumpMonitor. If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------GPLv3------------------------------------//
#include "netcdfdata.h"
#include <netcdf.h>
#include <QDateTime>
#include <QHostInfo>
#include <QFile>
#include <QProcess>
#include <QApplication>

#define NCCHECK(e){if(e!=NC_NOERR){printf("Error: %s\n", nc_strerror(e));fflush(stdout);return false;}}

NetcdfData::NetcdfData(QString filename, bool useFloat, bool useEtape, bool useUltrasonic, QObject *parent) : QObject(parent) {
    this->m_filename = filename;
    this->m_useFloat = useFloat;
    this->m_useEtape = useEtape;
    this->m_useUltrasonic = useUltrasonic;
    this->m_ncid = 0;
    this->m_varidetape = 0;
    this->m_varidfloat = 0;
    this->m_varidultrasonic = 0;
    this->m_varidtime = 0;
    this->m_dimidtime = 0;
}

NetcdfData::~NetcdfData() {}

bool NetcdfData::initialize() {
    QFile file(this->m_filename);
    if(file.exists()){
        bool error = this->_checkFileFormat();
        if(!error){
            if(this->m_clobber){
                this->_createFile();
            }else{
                return false;
            }
        }
        return true;
    } else {
        this->_createFile();
        return true;
    }
}


bool NetcdfData::_checkFileFormat(){
  if(this->_open()){
      this->_close();
      return true;
  }
  else
      return false;
}


bool NetcdfData::_createFile(){
    
    //...Get environment
    QString username = this->_getCurrentUserName();
    QString machinename = QHostInfo::localHostName();
    QString createDate = QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss");
    QString refDate = this->m_referenceDate.toString("MM/dd/yyyy hh:mm:ss");
    
    //...Create the netCDF file
    NCCHECK(nc_create(this->m_filename.toStdString().c_str(),NC_NETCDF4,&this->m_ncid));
    NCCHECK(nc_def_dim(this->m_ncid,"datetime",NC_UNLIMITED,&this->m_dimidtime));
    NCCHECK(nc_def_var(this->m_ncid,"datetime",NC_LONG,1,&this->m_dimidtime,&this->m_varidtime));
    NCCHECK(nc_put_att(this->m_ncid,this->m_varidtime,"reference_date",NC_CHAR,refDate.length(),refDate.toStdString().c_str()));

    if(this->m_useFloat){
        NCCHECK(nc_def_var(this->m_ncid,"float_status",NC_INT,1,&this->m_dimidtime,&this->m_varidfloat));
        NCCHECK(nc_def_var_deflate(this->m_ncid,this->m_varidfloat,1,1,2));
    }

    if(this->m_useEtape){
        NCCHECK(nc_def_var(this->m_ncid,"etape_waterlevel",NC_DOUBLE,1,&this->m_dimidtime,&this->m_varidetape));
        NCCHECK(nc_def_var_deflate(this->m_ncid,this->m_varidetape,1,1,2));
    }

    if(this->m_useUltrasonic){
        NCCHECK(nc_def_var(this->m_ncid,"ultrasonic_waterlevel",NC_DOUBLE,1,&this->m_dimidtime,&this->m_varidultrasonic));
        NCCHECK(nc_def_var_deflate(this->m_ncid,this->m_varidultrasonic,1,1,2));
    }

    NCCHECK(nc_put_att(this->m_ncid,NC_GLOBAL,"user",NC_CHAR,username.length(),username.toStdString().c_str()));
    NCCHECK(nc_put_att(this->m_ncid,NC_GLOBAL,"host",NC_CHAR,machinename.length(),machinename.toStdString().c_str()));
    NCCHECK(nc_put_att(this->m_ncid,NC_GLOBAL,"creation_date",NC_CHAR,createDate.length(),createDate.toStdString().c_str()));

    NCCHECK(nc_enddef(this->m_ncid));

    NCCHECK(nc_close(this->m_ncid));

    return true;
}


bool NetcdfData::_open() {
  
  QFile file(this->m_filename);
  if(!file.exists()){
    return this->_createFile();
  }
        
  NCCHECK(nc_open(this->m_filename.toStdString().c_str(),NC_WRITE,&this->m_ncid));
  NCCHECK(nc_inq_dimid(this->m_ncid,"datetime",&this->m_dimidtime));
  NCCHECK(nc_inq_varid(this->m_ncid,"datetime",&this->m_varidtime));

  if(this->m_useFloat){
      NCCHECK(nc_inq_varid(this->m_ncid,"float_status",&this->m_varidfloat));
  }

  if(this->m_useEtape){
      NCCHECK(nc_inq_varid(this->m_ncid,"etape_waterlevel",&this->m_varidetape));
  }

  if(this->m_useUltrasonic){
      NCCHECK(nc_inq_varid(this->m_ncid,"ultrasonic_waterlevel",&this->m_varidultrasonic));
  }
  
  return true;

}


bool NetcdfData::_close() {
    NCCHECK(nc_close(this->m_ncid));
    return true;
}

bool NetcdfData::writeToFile(SumpData *data){
  static size_t size[] = {0};
  static long dt[] = {0};
  static double wl[] = {0};
  static int fl[] = {0};

  dt[0] = this->m_referenceDate.secsTo(data->time());
  wl[0] = data->waterLevel();

  bool err = this->_open();
  if(!err)
    return err;

  NCCHECK(nc_inq_dimlen(this->m_ncid,this->m_dimidtime,&size[0]));

  NCCHECK(nc_put_var1_long(this->m_ncid,this->m_varidtime,size,&dt[0]));

  if(this->m_useFloat) {
    if(data->floatStatus())
      fl[0] = 1;
    else
      fl[0] = 0;
    NCCHECK(nc_put_var1_int(this->m_ncid,this->m_varidfloat,size,&fl[0]));
  }
  
  if(this->m_useEtape) {
      NCCHECK(nc_put_var1_double(this->m_ncid,this->m_varidetape,size,&wl[0]));
  } else if(this->m_useUltrasonic) {
      NCCHECK(nc_put_var1_double(this->m_ncid,this->m_varidultrasonic,size,&wl[0]));
  }

  this->_close();
  return true;
}

QString NetcdfData::_getCurrentUserName() {
    QProcess process(this);
    process.setProgram("whoami");
    process.start();
    while(process.state() != QProcess::NotRunning)
        qApp->processEvents();
    return process.readAll().simplified();
}
