package com.union.test;

import com.union.atm.TermPack;
import com.union.atm.testATM;

public class DemoTest {
	/**
	 * @param args 测试3des算法 ，測試接口函數
	 * 密文长度必须是8，16，24，没有进行填充
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try 
		{ 
			String encoding = ("UTF-8"); 
			byte[] pack = { (byte) 0xc0, '2', 0x52, 64, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 't', 'e', 'r', 'm', '0', '1', '2', 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0x01, 0x23, 0x45, 0x67, (byte) 0x89,
					(byte) 0xab, (byte) 0xcd, (byte) 0xef, 0x01, 0x23, 0x45, 0x67,
					(byte) 0x89, (byte) 0xab, (byte) 0xcd, (byte) 0xef, 0x11, 0x22,
					0x33, 0x44, 0x55, 0x66, 0x77, (byte) 0x88, 16, 0x7D, 0x44,
					(byte) 0xCB, (byte) 0xE5, (byte) 0xD7, (byte) 0x82, (byte) 0xCA,
					(byte) 0xC8, 0x19, (byte) 0xAB, (byte) 0xAE, 0x1C, (byte) 0xD6,
					0x11, 0x03, 0x0A, 0x60, (byte) 0xC1, 0x52, (byte) 0xc0 };
			testATM tATM =new testATM();
			//第一个接口函数
			TermPack term=tATM.dispatchData(77,pack);			

			System.out.println(term);
			System.out.println(term.Deviceid);
			System.out.println(term.decryptedTMK);
	
			//第二个接口函数
			String time = "100312103048";
			byte[] timeText = time.getBytes("UTF-8"); 
			
			byte[] result =tATM.assembleMsg(1, timeText.length, timeText);	
                
            System.out.println("********************************************");            
            System.out.println(new String(timeText, encoding));   //(byte ,字符集) 
            System.out.println(bytesToString(timeText));          //byte[] 每个打印
            System.out.println("********************************************");   
            System.out.println(new String(result, encoding));   
            System.out.println(bytesToString(result));   
            System.out.println("********************************************");   
        } 
		catch (Exception e) 
		{   
            e.printStackTrace();   
        }   

	}
    private static String bytesToString(byte[] b) {   
        if (b == null)   
            return null;   
        StringBuffer sb = null;   
        sb = new StringBuffer();   
        for (int i = 0; i < b.length; i++) 
        {   
            sb.append(Integer.toString((int) ((b[i] >> 4) & 0x0F), 16));   
            sb.append(Integer.toString((int) (b[i] & 0x0F), 16));   
        }   
        return sb.toString().toUpperCase();   
    }   

}
