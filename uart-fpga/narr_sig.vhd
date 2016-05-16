--------------------------------------------
-- 信号窄化器
-- 在sig_in的上升沿作为启动窄化的信号
-- 窄化的后的信号持续narr_prd个clk周期的高电平
-- 当narr_prd=0时, narr_sig_out就是0了
--------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity narr_sig is
  port(
    sig_in       : in  std_logic;
    clk          : in  std_logic;
    reset        : in  std_logic;
    narr_prd     : in  std_logic_vector(7 downto 0); -- narr信号持续的周期数(以clk为周期)
    narr_sig_out : out std_logic
  );
end narr_sig;

architecture arch of narr_sig is
  -- narr信号持续的周期计数器(1 -> narr_prd)
  signal narr_prd_cnt   : std_logic_vector(7 downto 0);
  -- 停止窄化标志 0-窄化 1-不窄化
  signal stop_narr_flag : std_logic;
begin
  process(sig_in, reset, clk, stop_narr_flag)
  begin
    if reset = '1' then
      narr_prd_cnt   <= X"00";
      stop_narr_flag <= '1';
		narr_sig_out   <= '0';
    else
      if sig_in = '0' then
        narr_prd_cnt   <= X"00";
        stop_narr_flag <= '0';
        narr_sig_out   <= '0';
      else
        if stop_narr_flag = '0' then
          if rising_edge(clk) then
            narr_prd_cnt <= narr_prd_cnt + '1';
            narr_sig_out <= '1';
            if narr_prd_cnt = narr_prd then
              stop_narr_flag <= '1';
              narr_prd_cnt   <= X"00";
              narr_sig_out   <= '0';
            end if;
          end if;
        end if;
      end if;
    end if;
  end process;
end arch;

