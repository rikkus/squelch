extern "C"
{
  void dcopCallback
  (
   const char * oid,
   const char * fn,
   const char * d,
   unsigned int l
  )
  {
    QString objectID(QString::fromUtf8(oid));
    QString function(QString::fromUtf8(fn));
    QByteArray data;
    data.duplicate(d, l);

    if ("play" == function)
    {
      qDebug("play");
    }
    else if ("stop" == function)
    {
      qDebug("stop");
    }
  }
}

