--------------------------------------------
-- 串口收发实验
-- Filename: uart
-- PIN_89--->P1.2
--------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity uart is -- 定义uart实体
  port(
    clkin, resetin : in  std_logic;                    -- clkin 为50M
    rxd            : in  std_logic;                    -- 串行输入数据
    txd            : out std_logic;                    -- 串行输出数据
    wei            : out std_logic_vector(3 downto 0); -- S0 ~ S3
    duan           : out std_logic_vector(7 downto 0) 
  );
end uart;

architecture arch of uart is
  component gen_div is -- 分频元件调用声明
    -- 326分频, 326 * 16 * (9600) = 50M
    -- 波特率为9600 
    generic(div_param : integer := 163);
    port(
      clk_in  : in  std_logic;
      clk_out : out std_logic;
      reset   : in  std_logic
    );
  end component;

  component uart_send is -- 串口发送元件调用声明
    port(
      bclk_t, reset_t, xmit_cmd_p : in  std_logic;
      tbuf                        : in  std_logic_vector(7 downto 0);
      txd                         : out std_logic;
      t_done                      : out std_logic
    );
  end component;

  component uart_recv is -- 串口接受元件调用声明
    port(
      bclk_r, reset_r, rxd : in  std_logic;
      r_ready              : out std_logic;
      rbuf                 : out std_logic_vector(7 downto 0)
    );
  end component;

  component narr_sig is -- 信号窄化元件声明调用
    port(
      sig_in       : in  std_logic;
      clk          : in  std_logic;
      reset        : in  std_logic;
      narr_prd     : in  std_logic_vector(7 downto 0);
      narr_sig_out : out std_logic
    );
  end component;

  signal clk_b : std_logic; -- 波特率时钟
  signal clk1  : std_logic; -- 数码管时钟

  signal xmit_p        : std_logic;                    -- 新一轮发送启动信号
  signal xbuf          : std_logic_vector(7 downto 0); -- 待发送数据缓冲区
  signal txd_done_iner : std_logic;                    -- 帧数据发送完标志

  signal rev_buf   : std_logic_vector(7 downto 0); -- 接收数据缓冲区
  signal rev_ready : std_logic;                    -- 帧数据接受完标志
begin
  ---------------------------------
  -- 分频模块例化
  ---------------------------------
  uart_baud: gen_div
    generic map(163)
    port map(
      clk_in  => clkin,
      reset   => not resetin,
      clk_out => clk_b
    );
  ---------------------------------
  -- 分频模块例化
  ---------------------------------
  seg_clk: gen_div
    generic map(10) -- 20分频
    port map(
      clk_in  => clkin,
      reset   => not resetin,
      clk_out => clk1
    );
  ---------------------------------
  -- 串口发送模块例化
  ---------------------------------
  uart_transfer: uart_send
    port map(
      bclk_t     => clk_b,
      reset_t    => not resetin,
      xmit_cmd_p => xmit_p,
      tbuf       => xbuf,
      txd        => txd,
      t_done     => txd_done_iner
    );
  ---------------------------------
  -- 串口接收元件例化
  ---------------------------------
  uart_receive: uart_recv
    port map(
      bclk_r  => clk_b,
      reset_r => not resetin,
      rxd     => rxd,
      r_ready => rev_ready,
      rbuf    => rev_buf
     );
  ---------------------------------
  -- 信号窄化模块例化
  ---------------------------------
  narr_rev_ready: narr_sig -- 窄化rev_ready信号后给xmit_p
    port map(
      sig_in       => rev_ready,   -- 输入需窄化信号
      clk          => clk_b,
      reset        => not resetin,
      narr_prd     => X"03",       -- narr信号高电平持续的周期数(以clk为周期)
      narr_sig_out => xmit_p       -- 输出窄化后信号
    );

  process(rev_ready, resetin, rev_buf, clk_b)
  begin
    if rising_edge(rev_ready) then -- 接收完毕
      xbuf <= rev_buf; -- 装载数据		 
    end if;
  end process;

  display: process(clk1, rev_ready, rev_buf)
  begin
    if rising_edge(rev_ready) then  -- 接收完毕
      wei <= rev_buf(7 downto 4);   -- 前四位为位选信息     
      case rev_buf(3 downto 0) is   -- 后四位为位选信息
      when "0000" => duan <= x"3f"; -- 0
      when "0001" => duan <= x"06"; -- 1
      when "0010" => duan <= x"5b"; -- 2
      when "0011" => duan <= x"4f"; -- 3
      when "0100" => duan <= x"66"; -- 4
      when "0101" => duan <= x"6d"; -- 5
      when "0110" => duan <= x"7d"; -- 6
      when "0111" => duan <= x"07"; -- 7
      when "1000" => duan <= x"7f"; -- 8
      when "1001" => duan <= x"6f"; -- 9
      when "1010" => duan <= x"bf"; -- 0.
      when "1011" => duan <= x"86"; -- 1.
      when "1100" => duan <= x"db"; -- 2.
      when "1101" => duan <= x"cf"; -- 3.
      when "1110" => duan <= x"e6"; -- 4.
      when "1111" => duan <= x"ed"; -- 5.
      end case;
    end if;
  end process;
end arch;
	 