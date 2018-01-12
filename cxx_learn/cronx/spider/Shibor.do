<!--
  * Project:      Shibor System
  * Description:  shibor数据显示
  * Module:
  * Copyright:    Copyright (c) 2006
  * Company:      FuDan Kingstar Corp.
  * @author       yuyu
  * @version      0.1
  *
  **************************************************************
  * Create:       2006-8-25  15:42:21
  **************************************************************
  * Modified Histroy
  *
-->





<LINK href="/shibor/web/css/normal/newCss/tab_menu.css" type=text/css rel=stylesheet>
<link href="/shibor/web/css/normal/newCss/style.css" rel="stylesheet" type="text/css">
<link href="/shibor/web/css/normal/newCss/class.css" rel="stylesheet" type="text/css">
<SCRIPT src="/shibor/web/script/prototype.js" type=text/javascript></SCRIPT>
<SCRIPT src="/shibor/web/script/fabtabulous.js" type=text/javascript></SCRIPT>
<SCRIPT src="/shibor/web/script/upslip.js" type=text/javascript></SCRIPT>











<script language="javascript">
function checkNewMsg(){
	if(true){
    self.location.replace("/shibor/Shibor.do");
    }
}

function showInfo(){
	document.write('<table><tr><td>yuyu</td></tr></table>');
}
</script>


<body onload="javascript:window.setTimeout('checkNewMsg();',67553000);" style="background:#FFF">

<html:form  method="post" >
 <!-------------------------------------------------    data stage  ----------------------------------------------->
 
 <table width="98%" align="center" border="0" cellspacing="0" cellpadding="0">
  <tr>
  	<td td align="right" bgcolor="#ffffff" colspan="3" height="30" width="100%" valign="middle">
  		&nbsp;&nbsp;&nbsp;&nbsp;2016-03-23 09:30:00&nbsp;&nbsp;
  </tr>
  <tr>
	  <td height="2" colspan="3" valign="bottom" align="center">
	  		<table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td width="10%" height="30" align="center" class="infoTitleb">&nbsp;</td>
                  <td width="15%" align="right" nowrap="nowrap"><strong>&nbsp;&nbsp;&nbsp;&nbsp;期限</strong></td>
                  <td width="33%"  nowrap="nowrap"><strong>&nbsp;&nbsp;&nbsp;&nbsp;Shibor(%)</strong></td>
                  <td width="10%" height="30" align="center" class="infoTitleb">&nbsp;</td>
                  <td width="31%" align="center"  nowrap="nowrap"><strong>涨跌(BP)</strong></td>
                </tr>
                <tr>
                  <td height="5" colspan="5" bgcolor="c2d7e9"></td>
                </tr>
       		</table>
      <table width="100%" border="0" cellspacing="0" cellpadding="0" class="shiborquxian" >
      
     
					<tr>
		                  <td width="10%" align="center" height="30"><img src="/shibor/web/images/newimages/shibor_arrow.gif" width="11" height="12"></td>
		                  <td width="15%" align="center"><a href="#" onClick="window.open('/shibor/shiborChartShow.do?termId=O/N','走势图','width=800,height=550,top=120,left='+(screen.availWidth/2-405)+',toolbar=no,menubar=no,scrollbars=yes, resizable=yes,location=no, status=no')" title="Shibor走势图"   ><font color=blue>O/N</font></a></td>
		                  <td width="33%" align="center">2.0000</td>
		                  <td width="10%" align="center"><img src="/shibor/web/images/newimages/downicon.gif" width="11" height="11" /></td>
		                  <td width="31%" align="center">0.70</td>
		            </tr>	
					<tr>
		                  <td width="10%" align="center" height="30"><img src="/shibor/web/images/newimages/shibor_arrow.gif" width="11" height="12"></td>
		                  <td width="15%" align="center"><a href="#" onClick="window.open('/shibor/shiborChartShow.do?termId=1W','走势图','width=800,height=550,top=120,left='+(screen.availWidth/2-405)+',toolbar=no,menubar=no,scrollbars=yes, resizable=yes,location=no, status=no')" title="Shibor走势图"   ><font color=blue>1W</font></a></td>
		                  <td width="33%" align="center">2.3010</td>
		                  <td width="10%" align="center"><img src="/shibor/web/images/newimages/downicon.gif" width="11" height="11" /></td>
		                  <td width="31%" align="center">0.40</td>
		            </tr>	
					<tr>
		                  <td width="10%" align="center" height="30"><img src="/shibor/web/images/newimages/shibor_arrow.gif" width="11" height="12"></td>
		                  <td width="15%" align="center"><a href="#" onClick="window.open('/shibor/shiborChartShow.do?termId=2W','走势图','width=800,height=550,top=120,left='+(screen.availWidth/2-405)+',toolbar=no,menubar=no,scrollbars=yes, resizable=yes,location=no, status=no')" title="Shibor走势图"   ><font color=blue>2W</font></a></td>
		                  <td width="33%" align="center">2.6250</td>
		                  <td width="10%" align="center"><img src="/shibor/web/images/newimages/upicon.gif" width="11" height="11" /></td>
		                  <td width="31%" align="center">0.90</td>
		            </tr>	
					<tr>
		                  <td width="10%" align="center" height="30"><img src="/shibor/web/images/newimages/shibor_arrow.gif" width="11" height="12"></td>
		                  <td width="15%" align="center"><a href="#" onClick="window.open('/shibor/shiborChartShow.do?termId=1M','走势图','width=800,height=550,top=120,left='+(screen.availWidth/2-405)+',toolbar=no,menubar=no,scrollbars=yes, resizable=yes,location=no, status=no')" title="Shibor走势图"   ><font color=blue>1M</font></a></td>
		                  <td width="33%" align="center">2.7025</td>
		                  <td width="10%" align="center"><img src="/shibor/web/images/newimages/upicon.gif" width="11" height="11" /></td>
		                  <td width="31%" align="center">0.35</td>
		            </tr>	
					<tr>
		                  <td width="10%" align="center" height="30"><img src="/shibor/web/images/newimages/shibor_arrow.gif" width="11" height="12"></td>
		                  <td width="15%" align="center"><a href="#" onClick="window.open('/shibor/shiborChartShow.do?termId=3M','走势图','width=800,height=550,top=120,left='+(screen.availWidth/2-405)+',toolbar=no,menubar=no,scrollbars=yes, resizable=yes,location=no, status=no')" title="Shibor走势图"   ><font color=blue>3M</font></a></td>
		                  <td width="33%" align="center">2.7980</td>
		                  <td width="10%" align="center"><img src="/shibor/web/images/newimages/upicon.gif" width="11" height="11" /></td>
		                  <td width="31%" align="center">0.10</td>
		            </tr>	
					<tr>
		                  <td width="10%" align="center" height="30"><img src="/shibor/web/images/newimages/shibor_arrow.gif" width="11" height="12"></td>
		                  <td width="15%" align="center"><a href="#" onClick="window.open('/shibor/shiborChartShow.do?termId=6M','走势图','width=800,height=550,top=120,left='+(screen.availWidth/2-405)+',toolbar=no,menubar=no,scrollbars=yes, resizable=yes,location=no, status=no')" title="Shibor走势图"   ><font color=blue>6M</font></a></td>
		                  <td width="33%" align="center">2.8840</td>
		                  <td width="10%" align="center"><img src="/shibor/web/images/newimages/downicon.gif" width="11" height="11" /></td>
		                  <td width="31%" align="center">0.30</td>
		            </tr>	
					<tr>
		                  <td width="10%" align="center" height="30"><img src="/shibor/web/images/newimages/shibor_arrow.gif" width="11" height="12"></td>
		                  <td width="15%" align="center"><a href="#" onClick="window.open('/shibor/shiborChartShow.do?termId=9M','走势图','width=800,height=550,top=120,left='+(screen.availWidth/2-405)+',toolbar=no,menubar=no,scrollbars=yes, resizable=yes,location=no, status=no')" title="Shibor走势图"   ><font color=blue>9M</font></a></td>
		                  <td width="33%" align="center">2.9800</td>
		                  <td width="10%" align="center"><img src="/shibor/web/images/newimages/downicon.gif" width="11" height="11" /></td>
		                  <td width="31%" align="center">0.50</td>
		            </tr>	
					<tr>
		                  <td width="10%" align="center" height="30"><img src="/shibor/web/images/newimages/shibor_arrow.gif" width="11" height="12"></td>
		                  <td width="15%" align="center"><a href="#" onClick="window.open('/shibor/shiborChartShow.do?termId=1Y','走势图','width=800,height=550,top=120,left='+(screen.availWidth/2-405)+',toolbar=no,menubar=no,scrollbars=yes, resizable=yes,location=no, status=no')" title="Shibor走势图"   ><font color=blue>1Y</font></a></td>
		                  <td width="33%" align="center">3.0555</td>
		                  <td width="10%" align="center"><img src="/shibor/web/images/newimages/downicon.gif" width="11" height="11" /></td>
		                  <td width="31%" align="center">0.25</td>
		            </tr>	
		
		
			</table >
                <table width="100%" height="50" border="0" cellpadding="0" cellspacing="0" >
                  <tr>
                    <td valign="top"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                      <tr>
                        <td height="30" align="center"></td>
                      </tr>
                      <tr>
                        <td align="center"><STRONG>提示：</STRONG>点击期限品种查看历史走势</td>
                      </tr>
                      
                    </table></td>
                    </tr>
                </table>
                
                </td>
            </tr>
          </table>
</html:form>
</body>

