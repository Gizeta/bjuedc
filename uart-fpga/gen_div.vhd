--------------------------------------------
-- 通用偶数分频器
--------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity gen_div is
  -- 分频因子, 分频为2*div_param, 默认2分频
  generic(div_param : integer := 1);
  port(
    clk_in  : in  std_logic; -- 输入时钟
    clk_out : out std_logic; -- 分频输出
    reset   : in  std_logic  -- 复位信号
  );
end gen_div;

architecture arch of gen_div is
  signal tmp : std_logic;                         -- 输出暂存寄存器
  signal cnt : integer range 0 to div_param := 0; -- 计数寄存器
begin
  process(clk_in, reset)
  begin
    if reset = '1' then -- reset有效时, output始终是0
      cnt <= 0;
      tmp <= '0';
    elsif rising_edge(clk_in) then
      cnt <= cnt + 1;
      if cnt = div_param - 1 then
        tmp <= not tmp; -- 取反信号
        cnt <= 0;
      end if;
    end if;
  end process;
  clk_out <= tmp; -- 输出
end arch;

