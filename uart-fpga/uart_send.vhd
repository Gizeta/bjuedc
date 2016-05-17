--------------------------------------------
-- 串口发送模块
--------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity uart_send is
  generic(framelen : integer := 7); -- 数据位为 framelen + 1 位
  port(
    -- bclk - 发送时钟
    -- reset - 复位信号
    -- xmit_cmd_p - 新一轮发送启动信号
    bclk_t, reset_t, xmit_cmd_p : in  std_logic;
    -- 待发送数据寄存器
    tbuf                        : in  std_logic_vector(7 downto 0);
    -- 串行数据输出
    txd                         : out std_logic;
    -- 一帧数据(8bits)发送完毕标志
    t_done                      : out std_logic
  );
end uart_send;

architecture arch of uart_send is
  -- 状态机状态
  type states is (t_idle, t_start_bit, t_data_bit, t_stop_bit);
  -- 初始状态为t_idle
  signal state : states := t_idle;
begin
  process(bclk_t, reset_t, xmit_cmd_p, tbuf)
    -- 发送1bit所要保持的时钟计数器
    -- (因为现在的bclk_t是baud频率的16倍)
    variable count   : std_logic_vector(4 downto 0) := "00000";
    -- 已经发送的数据位 计数器
    variable tcnt    : integer range 0 to framelen := 0;
    -- 串行输出数据暂存变量
    variable txd_tmp : std_logic;	
  begin
    if reset_t = '1' then 
      state   <= t_idle;
      t_done  <= '0';
      txd_tmp := '1';
      tcnt    := 0;
      count   := "00000";
    elsif rising_edge(bclk_t) then
      case state is
        when t_idle =>
          if xmit_cmd_p = '1' then -- 判断是否启动新一轮发送
            state <= t_start_bit;  -- 准备发送起始位
            -- 直到有这个发送脉冲后, t_done才复位
            -- 从这儿开始, 发送数据不能改变了
            t_done  <= '0'; 
            txd_tmp := '0';
            count   := "00000";
          else 
            state   <= t_idle;
            txd_tmp := '1';
          end if;
        when t_start_bit =>        -- 发送起始位
          if count <= "01110" then -- 需要保持16个时钟
            count   := count + 1;
            txd_tmp := '0';
            state   <= t_start_bit; -- 继续发送起始位	
          else
            state   <= t_data_bit; -- 准备开始发送数据位
            count   := "00000";    -- 重置为0
            txd_tmp := tbuf(0);    -- 发送第0位
            tcnt    := 0;
          end if;
        when t_data_bit => -- 发送数据位
          if count <= "01110" then
            count := count + 1;
            state <= t_data_bit;
          else
            if tcnt = framelen then -- 判断是否已经发送完一帧(8bits)数据
              state   <= t_stop_bit;
              tcnt    := 0;
              count   := "00000";
              txd_tmp := '1';
            else
              tcnt    := tcnt + 1;
              txd_tmp := tbuf(tcnt);
              state   <= t_data_bit;
              count   := "00000"; -- 重新计数一bit所要保持的时间
            end if;
          end if;
        when t_stop_bit => -- 停止位也是16位
          if count <= "01110" then
            count   := count + 1;
            txd_tmp := '1';
            state   <= t_stop_bit;
          else 					
            state   <= t_idle;
            count   := "00000";
            txd_tmp := '1';
            t_done  <= '1';
          end if;					
        when others => -- 回到t_idle状态
          state <= t_idle;								
        end case;
      end if;
    txd <= txd_tmp ; -- 当txd_tmp变化时, txd就立即变化, 功能仿真时无延时
  end process;
end arch;
